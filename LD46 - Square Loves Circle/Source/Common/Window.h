#ifndef WINDOW_H
#define WINDOW_H

//#include <SDL.h>
#include "Audio.h"
#include "../Templates/Event.h"
#include "../Templates/Dynamic.h"
#include "../Templates/Vector.h"
#include <cstdint>
#include <future>
#include <vector>
#include "../Templates/RawBuffer.h"
#include "../Templates/Shapes.h"
#include <complex>

void ensureInit();
struct SpriteVertex{
    Vector2<float> pos;
    Vector2<float> texturePos;
    Vector<float, 4> color;
};
using Index = std::uint32_t;
struct RenderingSegment{
    std::vector<SpriteVertex> vertices;
    std::vector<Index> indices;
    void addRectangle(Rectangle<float> const& position, Rectangle<float> const& texture, Vector<float, 4> const& color = {1.f, 1.f, 1.f, 1.f});
    void addRotatedRectangle(Vector<float, 2> translation, std::complex<float> const& rotation, Rectangle<float> const& position, Rectangle<float> const& texture, Vector<float, 4> const& color = {1.f, 1.f, 1.f, 1.f});
};
struct UniformValues{
    Vector2<float> offset;
    Vector2<float> scale;
};
class Texture{
    struct Impl;
    std::shared_ptr<Impl> data;
    friend class Window;
public:
    bool operator==(std::nullptr_t) const{ return data == nullptr; }
    bool operator!=(std::nullptr_t) const{ return data != nullptr; }
    operator bool() const{ return data != nullptr; }
};
template<typename R>
bool isFutureReady(std::future<R> const& f){
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

class Window{
public:
    SharedDynamic<double> time;
    SharedDynamic<Vector2<int> > extent;
    SharedDynamic<Vector2<int> > mousePosition;
    SharedDynamic<bool> leftMouseDown;
    SharedDynamic<bool> rightMouseDown;
    struct KeyEvent{
        enum Type{
            PRESS, REPEAT, RELEASE
        };
        Type type;
        int scancode;
        int key;
        int mods;
    };
    Event<KeyEvent> keyEvents;
    struct MouseEvent{
        enum Button{
            LEFT, RIGHT
        };
        enum Type{
            PRESS, RELEASE
        };
        Button button;
        Type type;
    };
    Event<MouseEvent> mouseButtonEvents;
    Event<> frameStart;
    Event<> firstInteraction; //To start audio!
    std::future<RawBuffer> readFile(std::string const& directory);
    std::future<Texture> loadTexture(std::string const& directory);
    std::future<SharedSample> loadMP3(std::string const& directory);
    std::future<SharedMonoSample> loadWAV(std::string const& directory);
    void readFileCallback(std::string const& directory, std::function<void(RawBuffer)> callback); //Always called on frameStart
    template<class T, class Callback>
    void onFutureReady(std::future<T> future, Callback&& callback){
        frameStart.hook(copyToIndirectFunction([future = std::move(future), callback = std::forward<Callback>(callback)]() mutable{
            if(isFutureReady(future)){
                callback(future.get());
                return EventAction::DISCONNECT;
            }else{
                return EventAction::NORMAL;
            }
        }));
    }
    template<class Callback>
    void readFileCallback(std::string const& directory, Callback&& callback){ onFutureReady(readFile(directory), std::forward<Callback>(callback)); }
    
    Window(unsigned int width, unsigned int height);
    ~Window();
    bool framePreamble();
    void framePostamble();
    
    //Rendering
    void clear(Vector<float, 3> const& color);
    void draw(RenderingSegment const& segment);
    void setUniforms(UniformValues const& values);
    void setTexture(Texture const& texture);
    enum struct BlendMode{
        NORMAL, ADD
    };
    void setBlendMode(BlendMode);
    
    //Audio
    bool audioReady();
    void playSample(SharedSample const&, float volume = 1.f, bool repeat = false);
    void playSample(SharedMonoSample const&, float lvolume, float rvolume, bool repeat = false);
    void playSample(SharedMonoSample const& sample, float volume = 1.f, bool repeat = false){
        playSample(sample, volume, volume, repeat);
    }
    float getAspectRatio(){
        return extent->get()[0] / (float)extent->get()[1];
    }
private:
    struct Impl;
    std::unique_ptr<Impl> data;
};

#endif
