
#include "Common/Window.h"

#include "frame.hpp"

Window& getWindow(){
    static Window window(640, 480);
    return window;
}
Program& getProgram(){
    static Program program(getWindow());
    return program;
}
#ifdef COMPILE_FOR_EMSCRIPTEN //Only define these functions for Emscripten!
#include <emscripten.h>

void mainLoop(){
    Window& window = getWindow();
    Program& program = getProgram();
    if(window.framePreamble()){
        program.frame(window);
        window.framePostamble();
    }
}
int main(){
    Window& window = getWindow();
    if(!window.framePreamble()) return 0;
    emscripten_set_main_loop(&mainLoop, 0, true);
}

#else
int main(){
    Window& window = getWindow();
    if(!window.framePreamble()) return 0;
    Program& program = getProgram();
    while(window.framePreamble()){
        program.frame(window);
        window.framePostamble();
    }
}
#endif

/*
#include <functional>

#include <emscripten.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

// Shader sources
const GLchar* vertexSource =
"attribute vec2 inPosition;                   \n"
"attribute vec2 inCoord;                      \n"
"attribute vec4 inColor;                      \n"
"varying vec2 outCoord;                       \n"
"varying vec4 outColor;                       \n"
"void main()                                  \n"
"{                                            \n"
"  gl_Position = vec4(inPosition.xy, 0.0, 1.0);\n"
"  outCoord = inCoord;                        \n"
"  outColor = inColor;                        \n"
"}                                            \n";
const GLchar* fragmentSource =
"precision mediump float;                     \n"
"varying vec2 outCoord;                       \n"
"varying vec4 outColor;                       \n"
"void main()                                  \n"
"{                                            \n"
"   gl_FragColor = outColor;                  \n"
"}                                            \n";

// an example of something we will control from the javascript side
bool background_is_black = true;

// the function called by the javascript code
extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color() { background_is_black = !background_is_black; }

std::function<void()> loop;
void main_loop() { loop(); }

int main()
{
    SDL_Window *window;
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, nullptr);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    
    GLfloat vertices[] = {0.0f, 0.5f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
                          0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
                          -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f};
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    
    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    
    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    // Specify the layout of the vertex data
    loop = [&]
    {
        // move a vertex
        const uint32_t milliseconds_since_start = SDL_GetTicks();
        const uint32_t milliseconds_per_loop = 3000;
        vertices[0] = ( milliseconds_since_start % milliseconds_per_loop ) / float(milliseconds_per_loop) - 0.5f;
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // Clear the screen
        if( background_is_black )
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        else
            glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        SDL_GL_SwapWindow(window);
    };
    
    emscripten_set_main_loop(main_loop, 0, true);
    
    return EXIT_SUCCESS;
}
*/
