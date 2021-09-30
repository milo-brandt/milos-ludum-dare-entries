
#include "Window.h"

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#include <iostream>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include "../../Dependencies/stb_image.h"


#ifdef COMPILE_FOR_EMSCRIPTEN //Ensure Dummy will not fully run (stop stupid programmers for getting mistaken)

namespace{
    const GLchar* vertexSource =
    "attribute vec2 inPosition;                   \n"
    "attribute vec2 inCoord;                      \n"
    "attribute vec4 inColor;                      \n"
    "varying vec2 outCoord;                       \n"
    "varying vec4 outColor;                       \n"
    "uniform vec2 offset;                          \n"
    "uniform vec2 scale;                           \n"
    "void main()                                  \n"
    "{                                            \n"
    "  gl_Position = vec4(inPosition.xy*scale + offset, 0.0, 1.0);\n"
    "  outCoord = inCoord;                        \n"
    "  outColor = inColor;                        \n"
    "}                                            \n";
    const GLchar* fragmentSource =
    "precision mediump float;                     \n"
    "varying vec2 outCoord;                       \n"
    "varying vec4 outColor;                       \n"
    "uniform sampler2D curTexture;                   \n"
    "void main()                                  \n"
    "{                                            \n"
    "   gl_FragColor = texture2D(curTexture, outCoord) * outColor;\n"
    "}                                            \n";
    bool ensureSDLStarted(){
        static bool check = [](){
            SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
            return true;
        }();
        return check; //Always will be true
    }
    static int check_compiled(GLuint shader) {
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        
        if(success == GL_FALSE) {
            GLint max_len = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);
            
            GLchar err_log[max_len];
            glGetShaderInfoLog(shader, max_len, &max_len, &err_log[0]);
            glDeleteShader(shader);
            
            fprintf(stderr, "Shader compilation failed: %s\n", err_log);
        }
        
        return success;
    }
    
    static int check_linked(GLuint program) {
        GLint success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        
        if(success == GL_FALSE) {
            GLint max_len = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_len);
            
            GLchar err_log[max_len];
            glGetProgramInfoLog(program, max_len, &max_len, &err_log[0]);
            
            fprintf(stderr, "Program linking failed: %s\n", err_log);
        }
        
        return success;
    }
    struct Renderer{
        GLuint ibo;
        GLuint vbo;
        GLuint scaleUniform;
        GLuint offsetUniform;
        GLuint textureBuf;
        Renderer(Window& window){
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ibo);

            GLfloat vertices[] = {0.0f, 0.5f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
                0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
                -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f};
            
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

            // Create and compile the vertex shader
            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexSource, nullptr);
            glCompileShader(vertexShader);
            check_compiled(vertexShader);
            
            // Create and compile the fragment shader
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
            glCompileShader(fragmentShader);
            check_compiled(fragmentShader);

            // Link the vertex and fragment shader into a shader program
            GLuint shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            check_linked(shaderProgram);
            glUseProgram(shaderProgram);
            {
                GLint posAttrib = glGetAttribLocation(shaderProgram, "inPosition");
                glEnableVertexAttribArray(posAttrib);
                glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 32, 0);
            }
            {
                GLint posAttrib = glGetAttribLocation(shaderProgram, "inCoord");
                if(posAttrib == -1) SDL_Log("Tragic");
                glEnableVertexAttribArray(posAttrib);
                glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 32, (void*)8);
            }
            {
                GLint posAttrib = glGetAttribLocation(shaderProgram, "inColor");
                if(posAttrib == -1) SDL_Log("Tragic2");
                glEnableVertexAttribArray(posAttrib);
                glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 32, (void*)16);
            }
            offsetUniform = glGetUniformLocation(shaderProgram, "offset");
            scaleUniform = glGetUniformLocation(shaderProgram, "scale");
            glUniform2f(offsetUniform, 0.f, 0.f);
            glUniform2f(scaleUniform, window.extent->get()[1] / (float)window.extent->get()[0], 1.f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    };
    

}
void ensureInit(){
    ensureSDLStarted();
}
struct Texture::Impl{
    GLuint glIndex;
    int width, height;
    ~Impl(){
        glDeleteTextures(1, &glIndex);
    }
};
struct Window::Impl{
    std::optional<Audio> audio;
    std::optional<Renderer> renderer;
    SDL_Window* native;
    SDL_GLContext glContext;
    unsigned int width;
    unsigned int height;
    bool interacted = false;
    Impl(Window& window, unsigned int width, unsigned int height):width(width),height(height){
        ensureSDLStarted();
        
#ifdef COMPILE_FOR_EMSCRIPTEN
        SDL_CreateWindowAndRenderer(width, height, 0, &native, nullptr);
        
        /*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);*/

        /*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);*/
#else
        
        native = SDL_CreateWindow("LD46", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
   
#endif
        //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        
        if(!(glContext = SDL_GL_CreateContext(native)))
            SDL_Log("Failed to get GL context: %s", SDL_GetError());
        if(SDL_GL_MakeCurrent(native, glContext) < 0)
            SDL_Log("Failed to make GL context current: %s", SDL_GetError());
        //SDL_GL_SetSwapInterval(1);
        
        window.extent->set({width, height});
        window.firstInteraction.hook([this](){
            audio.emplace();
        });
        renderer.emplace(window);
    }
    bool framePreamble(Window& window){
        SDL_Event event;
        /*
         Set Dynamics
         */
        window.time->set(SDL_GetTicks() / 1000.);
        {
            int x,y;
            auto buttons = SDL_GetMouseState(&x, &y);
            window.mousePosition->set({x,y});
            window.leftMouseDown->setIfNotEqual(buttons & SDL_BUTTON(SDL_BUTTON_LEFT));
            window.rightMouseDown->setIfNotEqual(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT));
        }
        
        bool priorInteracted = interacted;
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) return false;
            if(event.type == SDL_QUIT) return false;
            if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
                window.keyEvents.fire({
                    event.type == SDL_KEYUP ? KeyEvent::RELEASE : event.key.repeat ? KeyEvent::REPEAT : KeyEvent::PRESS,
                    event.key.keysym.scancode,
                    event.key.keysym.sym,
                    event.key.keysym.mod
                });
            }
            if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP){
                interacted = true;
                bool lmb = event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT);
                window.mouseButtonEvents.fire({
                    lmb ? MouseEvent::LEFT : MouseEvent::RIGHT,
                    event.type == SDL_MOUSEBUTTONDOWN ? MouseEvent::PRESS : MouseEvent::RELEASE
                });
            }
        }
        if(!priorInteracted && interacted){
            window.firstInteraction.fire();
        }
        window.frameStart.fire();
        return true;
    }
    void framePostamble(Window&){
        SDL_GL_SwapWindow(native);
    }
    void clear(Vector<float, 3> const& color){
        glClearColor(color[0], color[1], color[2], 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void draw(RenderingSegment const& segment){
        if(!renderer) return;
        
        glBufferData(GL_ARRAY_BUFFER, segment.vertices.size() * sizeof(SpriteVertex), &segment.vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, segment.indices.size() * sizeof(Index), &segment.indices[0], GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, (int)segment.indices.size(), GL_UNSIGNED_INT, 0);
        
    }
    void setUniforms(UniformValues const& values){
        glUniform2f(renderer->offsetUniform, values.offset[0], values.offset[1]);
        glUniform2f(renderer->scaleUniform, values.scale[0], values.scale[1]);
    }
    
    ~Impl(){
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(native);
        SDL_Quit();
    }
    
    std::future<RawBuffer> readFile(std::string const& directory){
#ifdef COMPILE_FOR_EMSCRIPTEN //Only define these functions for Emscripten!
        struct Userdata{
            std::string directory;
            std::promise<RawBuffer> promise;
        };
        Userdata* userdata = new Userdata{directory};
        emscripten_async_wget_data(directory.c_str(), userdata, [](void* _userdata, void* buffer, int bufferLen){
            auto userdata = (Userdata*)_userdata;
            userdata->promise.set_value({copyBuffer, buffer, (std::size_t)bufferLen});
            delete userdata;
        }, [](void* _userdata){
            auto userdata = (Userdata*)_userdata;
            SDL_Log("Loading file %s failed", userdata->directory.c_str());
            userdata->promise.set_exception(std::make_exception_ptr(std::runtime_error("File not loaded.")));
            delete userdata;
        });
        return userdata->promise.get_future();
#else
        return std::async(std::launch::async, [directory](){
            std::ifstream input(directory, std::ios_base::binary | std::ios_base::in);
            if(!input.is_open()){
                SDL_Log("Loading file %s failed", directory.c_str());
                throw std::runtime_error("File not loaded.");
            }
            input.seekg(0, std::ios::end);
            std::size_t size = input.tellg();
            RawBuffer ret(size);
            input.seekg(0);
            input.read((char*)ret.data, size);
            return ret;
        });
#endif
    }
    static Texture loadImage(RawBuffer const& buffer) {
        int textureWidth, textureHeight, channels;
        uint8_t* imageData = stbi_load_from_memory(buffer.data, (int)buffer.length, &textureWidth, &textureHeight, &channels, STBI_rgb_alpha);
        if (!imageData) {
            std::cerr << "Error loading image" << std::endl;
            throw std::runtime_error("Image failed to load from buffer.");
        }
                
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(imageData);
        Texture ret;
        ret.data = std::shared_ptr<Texture::Impl>(new Texture::Impl{texture, textureWidth, textureHeight});
        return ret;
    }

    std::future<Texture> loadTexture(Window& window, std::string const& directory){
        std::promise<Texture> promise;
        auto ret = promise.get_future();
        window.readFileCallback(directory, [promise = std::move(promise)](RawBuffer buffer) mutable{
            promise.set_value(loadImage(buffer));
        });
        return ret;
    }
    void setTexture(Texture const& texture){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.data->glIndex);
    }
    std::future<SharedSample> loadMP3(Window& window, std::string const& directory){
        std::promise<SharedSample> promise;
        auto ret = promise.get_future();
        window.readFileCallback(directory, [promise = std::move(promise)](RawBuffer buffer) mutable{
            promise.set_value(convertToMP3(buffer));
        });
        return ret;
    }
    void playSample(Window& window, SharedSample sample, float volume, bool repeat){
        if(audio){
            audio->playSample(sample, volume, repeat);
        }
    }
    void playSample(Window& window, SharedMonoSample const& sample, float lvolume, float rvolume, bool repeat){
        if(audio){
            audio->playSample(sample, lvolume, rvolume, repeat);
        }
    }
    struct FileLoadResult{
        std::function<void(bool)> handler;
    };
    static std::unordered_map<std::string, FileLoadResult> map;
    static void registerFileLoadCallback(std::string directory, FileLoadResult result){
        assert(map.count(directory) == 0);
        map[directory] = std::move(result);
    }
    static void onFileLoaded(const char* filename){
        map[filename].handler(true);
        map.erase(filename);
    }
    static void onFileError(const char* filename){
        map[filename].handler(false);
        map.erase(filename);
    }
    std::future<SharedMonoSample> loadWAV(Window& window, std::string const& directory){
        std::promise<SharedMonoSample> promise;
        auto ret = promise.get_future();
        std::string trueDir = "/" + directory;
        registerFileLoadCallback(trueDir, {copyToIndirectFunction([directory, promise = std::move(promise)](bool success) mutable{
            if(success){
                promise.set_value(loadWAVFromFile(directory));
            }else{
                SDL_Log("Loading file %s failed", directory.c_str());
                promise.set_exception(std::make_exception_ptr(std::runtime_error("File not loaded.")));
            }
        })});
        emscripten_async_wget(directory.c_str(), directory.c_str(), &onFileLoaded, &onFileError);
        return ret;
    }
};
std::unordered_map<std::string, Window::Impl::FileLoadResult> Window::Impl::map;

Window::Window(unsigned int width, unsigned int height):data(new Impl(*this, width, height)){}
Window::~Window() = default;

bool Window::framePreamble(){ return data->framePreamble(*this); }
void Window::framePostamble(){ data->framePostamble(*this); }
void Window::clear(Vector<float, 3> const& color){ data->clear(color); }
std::future<RawBuffer> Window::readFile(std::string const& directory){
    return data->readFile(directory);
}
void Window::draw(RenderingSegment const& segment){ data->draw(segment); }
void Window::setUniforms(UniformValues const& values){ data->setUniforms(values); }
std::future<Texture> Window::loadTexture(std::string const& directory){ return data->loadTexture(*this, directory); }
std::future<SharedSample> Window::loadMP3(std::string const& directory){ return data->loadMP3(*this, directory); }
std::future<SharedMonoSample> Window::loadWAV(std::string const& directory){
    return data->loadWAV(*this, directory);
}

void Window::setTexture(Texture const& texture){ data->setTexture(texture); }
void Window::setBlendMode(BlendMode mode){
    if(mode == BlendMode::NORMAL)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    else{
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
}
bool Window::audioReady(){
    return data->audio.has_value();
}
void Window::playSample(SharedSample const& sample, float volume, bool repeat){ data->playSample(*this, sample, volume, repeat); }
void Window::playSample(SharedMonoSample const& sample, float lvolume, float rvolume, bool repeat){ data->playSample(*this, sample, lvolume, rvolume, repeat); }

void RenderingSegment::addRectangle(Rectangle<float> const& position, Rectangle<float> const& texture, Vector<float, 4> const& color){
    std::size_t index = vertices.size();
    vertices.push_back({position.lesser, texture.lesser, color});
    vertices.push_back({position.cornerLG(), texture.cornerLG(), color});
    vertices.push_back({position.cornerGL(), texture.cornerGL(), color});
    vertices.push_back({position.greater, texture.greater, color});
    indices.push_back(index);
    indices.push_back(index+1);
    indices.push_back(index+2);
    indices.push_back(index+2);
    indices.push_back(index+1);
    indices.push_back(index+3);
}
void RenderingSegment::addRotatedRectangle(Vector<float, 2> translation, std::complex<float> const& rotation, Rectangle<float> const& position, Rectangle<float> const& texture, Vector<float, 4> const& color){
    std::size_t index = vertices.size();
    auto transform = [&](Vector<float, 2> const& input){
        auto rot = rotation * std::complex<float>(input[0], input[1]);
        return Vector<float, 2>{rot.real() + translation[0], rot.imag() + translation[1]};
    };
    vertices.push_back({transform(position.lesser), texture.lesser, color});
    vertices.push_back({transform(position.cornerLG()), texture.cornerLG(), color});
    vertices.push_back({transform(position.cornerGL()), texture.cornerGL(), color});
    vertices.push_back({transform(position.greater), texture.greater, color});
    indices.push_back(index);
    indices.push_back(index+1);
    indices.push_back(index+2);
    indices.push_back(index+2);
    indices.push_back(index+1);
    indices.push_back(index+3);
}

#endif
