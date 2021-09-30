//
//  Audio.hpp
//  FactoryGame
//
//  Created by Milo Brandt on 2/14/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef Audio_hpp
#define Audio_hpp

#include "../Templates/SingleConsumerQueue.h"
#include "../Templates/RawBuffer.h"

#include <array>
#include <variant>
#include <memory>
#include <string>
#include <unordered_map>

struct MonoSample{
    short value;
};
struct Sample{
    std::array<short, 2> channels;
};
struct SharedSample{
    std::shared_ptr<Sample> samples;
    std::size_t len;
};
struct SharedMonoSample{
    std::shared_ptr<MonoSample> samples;
    std::size_t len;
};
struct MixingSample{
    std::array<int, 2> channels;
};
class Audio{
    struct Impl; //User data
    std::unique_ptr<Impl> impl;
    std::unordered_map<std::string, SharedMonoSample> loadedSamples;
public:
    Audio();
    ~Audio();
    unsigned int createBuffer();
    void playSample(SharedSample, float volume = 1.f, bool repeat = false);
    void playSample(SharedMonoSample, float lvolume, float rvolume, bool repeat = false);
    void playSample(SharedMonoSample sample, float volume = 1.f, bool repeat = false){
        playSample(std::move(sample), volume, volume, repeat);
    }

};
SharedMonoSample loadWAVFromFile(std::string filename);
SharedSample convertToMP3(RawBuffer const& buffer);

#endif /* Audio_hpp */
