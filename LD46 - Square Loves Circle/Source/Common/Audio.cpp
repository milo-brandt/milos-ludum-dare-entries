//
//  Audio.cpp
//  FactoryGame
//
//  Created by Milo Brandt on 2/14/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#include "Window.h"
#include "Audio.h"
#include <SDL.h>
#include <SDL_audio.h>
#include <vector>
#include <deque>
#include <iostream>

#define MINIMP3_ONLY_MP3
#define MINIMP3_IMPLEMENTATION
#include "../../Dependencies/minimp3.h"


namespace{
    static short clipIntFixedPoint(int fp){
        return short(fp / 65536);
    }
    static int floatToShort(float f){
        return int(f * 65536);
    }
    struct PlaySample{
        int scale;
        Sample* pos;
        std::size_t len;
        bool repeat;
        SharedSample sample;
    };
    struct PlayMonoSample{
        int lscale, rscale;
        MonoSample* pos;
        std::size_t len;
        bool repeat;
        SharedMonoSample sample;
    };
    using Playable = std::variant<PlaySample, PlayMonoSample>;
    struct ImplServerside{
        int bufferLength;
        SingleConsumerQueue<Playable> queue;

        std::vector<PlaySample> samples;
        std::vector<PlayMonoSample> monoSamples;

        
        static void callback(void* userdata, Uint8* _stream, int bytes){
            auto& me = *(ImplServerside*) userdata;
            me.queue.consume([&me](Playable playable){
                std::visit(overloaded{
                    [&me](PlaySample sample){
                        me.samples.push_back(std::move(sample));
                    },
                    [&me](PlayMonoSample sample){
                        me.monoSamples.push_back(std::move(sample));
                    }
                }, std::move(playable));
            });
            int sampleLen = bytes / sizeof(Sample);
            auto stream = (Sample*)_stream;
            for(int i = 0; i < sampleLen; ++i){
                int sums[2] = {0, 0};
                eraseFromVectorIf(me.samples, [&me, &sums](PlaySample& sample){
                    if(sample.len == 0){
                        if(sample.repeat){
                            me.queue.push(PlaySample{sample.scale, sample.sample.samples.get(), sample.sample.len, sample.repeat, sample.sample});
                        }
                        return true;
                    }
                    sums[0] += sample.pos->channels[0] * sample.scale;
                    sums[1] += sample.pos->channels[1] * sample.scale;
                    --sample.len;
                    ++sample.pos;
                    return false;
                });
                eraseFromVectorIf(me.monoSamples, [&me, &sums](PlayMonoSample& sample){
                    if(sample.len == 0){
                        if(sample.repeat){
                            me.queue.push(PlayMonoSample{sample.lscale, sample.rscale, sample.sample.samples.get(), sample.sample.len, sample.repeat, sample.sample});
                        }
                        return true;
                    }
                    sums[0] += sample.pos->value * sample.lscale;
                    sums[1] += sample.pos->value * sample.rscale;
                    --sample.len;
                    ++sample.pos;
                    return false;
                });
                stream[i].channels[0] = clipIntFixedPoint(sums[0]);
                stream[i].channels[1] = clipIntFixedPoint(sums[1]);
            }
        }
    };
}
struct Audio::Impl{
    ImplServerside server;
    int audioDeviceId;
    int bufferSize;
    //std::vector<MixingSample*> buffers;
    Impl(){
        ensureInit();
        SDL_AudioSpec wav_spec; // the specs of our piece of music
        // set the callback function
        wav_spec.freq = 44100;
        wav_spec.channels = 2;
        wav_spec.samples = 1024;
        wav_spec.callback = &ImplServerside::callback;
        wav_spec.userdata = &server;
        wav_spec.format = AUDIO_S16;
        SDL_AudioSpec ret;
        audioDeviceId = SDL_OpenAudioDevice(NULL, 0, &wav_spec, &ret, 0);
        if(audioDeviceId == -1){
            SDL_Log("Failed to open audio device %s.\n", SDL_GetError());
            std::terminate();
        }
        bufferSize = ret.samples;
        server.bufferLength = bufferSize;
        SDL_PauseAudioDevice(audioDeviceId, 0); //Start audio
    }
    ~Impl(){
        SDL_CloseAudioDevice(audioDeviceId);
    }
    /*
        std::size_t ret = buffers.size();
        auto ptr = std::make_unique<MixingSample[]>(bufferSize);
        buffers.push_back(ptr.get());
        server.queue.push(ServerMessages::AddBuffer{std::move(ptr)});
        return ret;
    }*/
};
Audio::Audio():impl(new Impl){}
Audio::~Audio() = default;
void Audio::playSample(SharedSample sample, float volume, bool repeat){
    impl->server.queue.push(PlaySample{floatToShort(volume), sample.samples.get(), sample.len, repeat, std::move(sample)});
}
void Audio::playSample(SharedMonoSample sample, float lvolume, float rvolume, bool repeat){
    impl->server.queue.push(PlayMonoSample{floatToShort(lvolume), floatToShort(rvolume), sample.samples.get(), sample.len, repeat, std::move(sample)});
}


template< typename T >
struct array_deleter
{
    void operator ()( T const * p)
    {
        delete[] p;
    }
};
SharedMonoSample loadWAVFromFile(std::string filename){
    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8 *wav_buffer;
    if (SDL_LoadWAV(filename.c_str(), &wav_spec, &wav_buffer, &wav_length) == NULL) {
        std::cout << "Could not open " << filename << ": " << SDL_GetError() << "\n";
        std::terminate();
    }
    //assert(wav_spec.channels == 2); //I forget why this is here, so it's possibly bad that I commented it out.
    SDL_AudioCVT converter;
    
    SDL_AudioSpec desired;
    desired.freq = 44100;
    desired.channels = 1;
    desired.format = AUDIO_S16;
    
    if(SDL_BuildAudioCVT(&converter, wav_spec.format, wav_spec.channels, wav_spec.freq, desired.format, desired.channels, desired.freq) < 0){
        std::cout << "Could not convert format.\n";
        std::cout << SDL_GetError();
        std::terminate();
    }
    converter.len = wav_length;
    if(converter.needed){
        converter.buf = (Uint8*)malloc(converter.len * converter.len_mult);
        memcpy(converter.buf, wav_buffer, wav_length);
        SDL_ConvertAudio(&converter);
    }else{
        converter.buf = wav_buffer;
    }
    auto sampleCount = (std::size_t)(converter.len_ratio * converter.len) / sizeof(MonoSample);
    SharedMonoSample ret{{new MonoSample[sampleCount], array_deleter<MonoSample>{}}, sampleCount};
    memcpy(ret.samples.get(), converter.buf, sampleCount * sizeof(MonoSample));
    SDL_FreeWAV(wav_buffer);
    if(converter.needed) free(converter.buf);
    return ret;
}
SharedSample convertToMP3(RawBuffer const& buffer){
    static mp3dec_t mp3d;
    mp3dec_init(&mp3d);
    mp3dec_frame_info_t info;
    short* pcm = new short[MINIMP3_MAX_SAMPLES_PER_FRAME];
    
    std::vector<short> decoded;
    std::size_t len = buffer.length;
    
    const unsigned char* ptr = (unsigned char*)buffer.data;
    while(len > 0){
        int samples = mp3dec_decode_frame(&mp3d, ptr, (int)len, pcm, &info);
        if(samples == 0) break;
        auto totalSamples = samples * info.channels;
        for(int i = 0; i < totalSamples; ++i){
            decoded.push_back(pcm[i]);
        }
        len -= info.frame_bytes;
        ptr += info.frame_bytes;
    }
    auto sampleCount = decoded.size()/2;
    SharedSample ret{{new Sample[sampleCount], array_deleter<Sample>{}}, sampleCount};
    memcpy(ret.samples.get(), &decoded[0], sizeof(Sample) * sampleCount);
    delete[] pcm;
    return ret;
}

