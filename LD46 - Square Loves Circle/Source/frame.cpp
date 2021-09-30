//
//  frame.cpp
//  Dummy
//
//  Created by Milo Brandt on 4/17/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#include "frame.hpp"
#include "Game/Physics.h"
#include <cmath>
#include "KeyboardHandler.h"
#include "Game/Random.hpp"
#include "Game/ImmediateData.h"
#include "Game/GameState.hpp"
#include "Game/CannedLevel.h"
#include "Transitions.hpp"
#include "FontRenderer.hpp"

namespace{
    struct TextScreen{
        std::string what;
        bool end = false;
        double fadeProgress = 0;
        double fadeDir = 1;
    };
    struct FadeIn{
        IndirectValue<GameState> active;
        RenderEffects::Effect fade{RenderEffects::LoveFade{{}, 0, 4, 1.5}};
    };
    struct Playing{
        IndirectValue<GameState> active;
    };
    struct FadeOut{
        RenderEffects::Effect fade;
        IndirectValue<GameState> active;
    };
    struct EndingScreen{
        double loveCounter = 0;
        std::vector<LoveData> loves;
        double fadeProgress = 0;
    };
    using ProgramState = std::variant<TextScreen, FadeIn, Playing, FadeOut, EndingScreen>;
    
    struct ShowText{
        std::string what;
        bool end = false;
        ProgramState convert() const{ return TextScreen{what, end}; }
    };
    struct PlayLevel{
        std::size_t which;
        ProgramState convert() const{
            IndirectValue<GameState> active;
            active->loadLevel(getLevel(which));
            return FadeIn{std::move(active)};
        }
    };
    struct TheEnd{
        ProgramState convert() const{ return EndingScreen{}; }
    };
    using ProgramStage = std::variant<ShowText, PlayLevel, TheEnd>;
}

struct Program::Impl{
    std::size_t levelPosition = 0;
    std::vector<ProgramStage> stages = {
        ShowText{"..."}, //You shouldn't see this frame! I don't know why, but you shouldn't!
        ShowText{"Square loves Circle."},
        ShowText{"Square keeps Circle from falling."},
        PlayLevel{0},
        ShowText{"Circle was constantly finding danger."},
        ShowText{"But Square kept Circle safe."},
        PlayLevel{1},
        ShowText{"Sometimes Circle went places Square couldn't."},
        ShowText{"But Square always found another way."},
        PlayLevel{2},
        ShowText{"Square was always there to keep Circle alive."},
        ShowText{"Even when made things complicated."},
        PlayLevel{3},
        ShowText{"Sometimes they had to go their own ways."},
        ShowText{"But this only made their reunion sweeter."},
        PlayLevel{4},
        ShowText{"Even when circumstance held them apart..."},
        ShowText{"...Square was always watching over Circle."},
        PlayLevel{5},
        ShowText{"Even when the odds seemed impossible..."},
        ShowText{"...Square made sure Circle would live."},
        PlayLevel{6},
        ShowText{"Circle always made Square happy."},
        ShowText{"Square felt like they were walking on clouds."},
        PlayLevel{7},
        ShowText{"Even when they had their disagreements."},
        ShowText{"Square remained loyal."},
        PlayLevel{8},
        ShowText{"Square loved Circle."},
        ShowText{"Square kept Circle alive."},
        ShowText{"Because that's what Squares do."},
        TheEnd{}
    };
    std::optional<Texture> font;
    ProgramState programState;
    std::optional<ImmediateData> data;
    bool dataReady = false;
    double lastUpdate = 0;
    std::unordered_map<Key, bool> keysHeld;
    bool waitingJump = false;
    bool waitingKeypress = false;
    ProgramState getStateFor(std::size_t position){
        if(position >= stages.size()) position = stages.size() - 1;
        waitingJump = false;
        waitingKeypress = false;
        return std::visit([](auto const& stage){ return stage.convert(); }, stages[position]);
    }
    void setupAfterData(Window& window){
        lastUpdate = window.time->get();
        window.playSample(data->backgroundMusic, 0.1f, true);
    }
    Impl(Window& window){
        //Make sure we can read from these!!!
        keysHeld[Key::LEFT] = false;
        keysHeld[Key::RIGHT] = false;
        keysHeld[Key::DOWN] = false;
        keysHeld[Key::SPACE] = false;
        keysHeld[Key::SKIP_LEVEL] = false;
        keysHeld[Key::RESTART] = false;

        window.onFutureReady(window.loadTexture("Resources/OpenSansFont.png"), [this](Texture _font){
            font = std::move(_font);
        });
        window.frameStart.hook(copyToIndirectFunction([
                                                        this,
                                                        &window,
                                                        white = window.loadTexture("Resources/White.png"),
                                                       circle = window.loadTexture("Resources/HappyCircle.png"),
                                                       love = window.loadTexture("Resources/Love.png"),
                                                       brokenL = window.loadTexture("Resources/BrokenLeft.png"),
                                                       brokenR = window.loadTexture("Resources/BrokenRight.png"),
                                                       loveFader = window.loadTexture("Resources/LoveFader.png"),
                                                       backgroundMusic = window.loadMP3("Resources/LD46Music.mp3"),
                                                       jump = window.loadWAV("Resources/Jump.wav"),
                                                       bounce = window.loadWAV("Resources/Hit.wav"),
                                                       death = window.loadWAV("Resources/Victory.wav"),
                                                      circleDeath = window.loadWAV("Resources/Failure.wav"),
                                                       click = window.loadWAV("Resources/Click.wav")
                                                      ]() mutable{
            if(isFutureReady(white) && isFutureReady(backgroundMusic) && isFutureReady(circle) && isFutureReady(love) && isFutureReady(jump) && isFutureReady(bounce) && isFutureReady(death) && isFutureReady(circleDeath) && isFutureReady(loveFader) && isFutureReady(brokenL) && isFutureReady(brokenR) && isFutureReady(click) && font){
                dataReady = true;
                if(window.audioReady()){
                    std::cout << "Required data loaded & audio started.\n";
                    data = ImmediateData{*font, white.get(), circle.get(), love.get(), brokenL.get(), brokenR.get(), loveFader.get(), backgroundMusic.get(), jump.get(), bounce.get(), death.get(), circleDeath.get(), click.get()};
                    setupAfterData(window);
                    return EventAction::DISCONNECT;
                }
            }
            return EventAction::NORMAL;
        }));
        window.mouseButtonEvents.hook([this](Window::MouseEvent const& event){
            if(event.type == Window::MouseEvent::PRESS) waitingKeypress = true;
        });
        window.keyEvents.hook([this](Window::KeyEvent const& event){
            if(event.type == Window::KeyEvent::REPEAT) return; //Don't care for inputs
            if(event.type == Window::KeyEvent::PRESS) waitingKeypress = true;
            auto key = processKeycode(event.key);
            if(key){
                keysHeld[*key] = event.type == Window::KeyEvent::PRESS;
                if(*key == Key::SPACE && event.type == Window::KeyEvent::PRESS){
                    waitingJump = true;
                }
            }
        });
    }
    void run(Window& window, double dt, TextScreen& screen){
        bool done = false;
        screen.fadeProgress += screen.fadeDir * dt;
        if(waitingKeypress){
            if(!screen.end){ //Don't respond on first frame
                screen.fadeDir = -1;
                waitingKeypress = false;
            }
        }

        done = screen.fadeProgress < 0;
        if(screen.fadeProgress > 1) screen.fadeProgress = 1;
        if(screen.fadeProgress < 0) screen.fadeProgress = 0;
        window.clear({0.18f, 0.08f, 0.01f});
        RenderingSegment segment;
        FontRenderer::generateDataToCenter(segment, screen.what, {0.01f, -0.01f}, 0.15f, {0.f, 0.f, 0.f, 1.f});
        FontRenderer::generateDataToCenter(segment, screen.what, {0.f, 0.f}, 0.15f, {1.f, .86f, .55f, 1.f});
        window.setTexture(*font);
        window.draw(segment);
        RenderingSegment cover;
        cover.addRectangle({{-2, -1}, {2, 1}}, standardTextureRect, {0.f, 0.f, 0.f, (float)(1-screen.fadeProgress)});
        window.setTexture(data->white);
        window.draw(cover);
        if(done){
            ++levelPosition;
            programState = getStateFor(levelPosition);
        }
    }
    void run(Window& window, double dt, EndingScreen& screen){
        bool done = false;
        screen.fadeProgress +=  dt;
        if(screen.fadeProgress > 1) screen.fadeProgress = 1;
        window.clear({0.18f, 0.08f, 0.01f});
        RenderingSegment segment;
        screen.loveCounter += dt;
        while(screen.loveCounter > 0.05){
            screen.loveCounter -= 0.05;
            screen.loves.emplace_back(getRandomInRectangle({{-1.4, -1}, {1.4, 1}}));
        }
        FontRenderer::generateDataToCenter(segment, "Made for Ludum Dare 46 by Milo Brandt", {0.01f, -0.01f}, 0.15f, {0.f, 0.f, 0.f, 1.f});
        FontRenderer::generateDataToCenter(segment, "Made for Ludum Dare 46 by Milo Brandt", {0.f, 0.f}, 0.15f, {1.f, .86f, .55f, 1.f});
        RenderingSegment segmentLove;
        eraseFromVectorIf(screen.loves, [&loves = segmentLove, dt](LoveData& data){
            return data.update(loves, dt);
        });
        window.setTexture(data->love);
        window.draw(segmentLove);
        window.setTexture(*font);
        window.draw(segment);
        RenderingSegment cover;
        cover.addRectangle({{-2, -1}, {2, 1}}, standardTextureRect, {0.f, 0.f, 0.f, (float)(1-screen.fadeProgress)});
        window.setTexture(data->white);
        window.draw(cover);
        if(done){
            ++levelPosition;
            programState = getStateFor(levelPosition);
        }
    }

    void run(Window& window, double dt, FadeIn& playing){
        playing.active->render(window, *data, {dt});
        if(RenderEffects::render(playing.fade, window, *data, dt)){
            programState = Playing{std::move(playing.active) };
        }else if(keysHeld[Key::SKIP_LEVEL]){
            keysHeld[Key::SKIP_LEVEL] = false;
            programState = getStateFor(++levelPosition);
        }
    }
    void run(Window& window, double dt, Playing& playing){
        InputInfo info;
        info.leftDown = keysHeld[Key::LEFT];
        info.rightDown = keysHeld[Key::RIGHT];
        info.upDown = keysHeld[Key::SPACE];
        info.downDown = keysHeld[Key::DOWN];
        info.jumped = waitingJump;
        auto notes = playing.active->update(dt, info);
        playing.active->render(window, *data, std::move(notes));
        if(playing.active->status == GameStatus::VICTORY){
            ++levelPosition;
            window.playSample(data->victory, 0.07f);
            programState = FadeOut{ //DESTROYS playing UPON ASSIGNMENT!
                RenderEffects::LoveFade{playing.active->physics.players[0]->center, 4, 0, 1.5},
                std::move(playing.active)
            };
        }else if(playing.active->status != GameStatus::NORMAL || keysHeld[Key::RESTART]){
            window.playSample(data->failure, 0.07f);
            programState = FadeOut{ //DESTROYS playing UPON ASSIGNMENT!
                RenderEffects::NormalFade{0, 1, 1.5},
                std::move(playing.active)
            };
        }else if(keysHeld[Key::SKIP_LEVEL]){
            keysHeld[Key::SKIP_LEVEL] = false;
            programState = getStateFor(++levelPosition);
        }
    }
    void run(Window& window, double dt, FadeOut& playing){
        InputInfo info;
        playing.active->update(dt, info);
        playing.active->render(window, *data, {dt});
        if(RenderEffects::render(playing.fade, window, *data, dt)){
            programState = getStateFor(levelPosition);
        }else if(keysHeld[Key::SKIP_LEVEL]){
            keysHeld[Key::SKIP_LEVEL] = false;
            programState = getStateFor(++levelPosition);
        }
    }
    void frame(Window& window){
        if(!data){
            window.clear({0.18f, 0.08f, 0.01f});
            if(font){
                RenderingSegment segment;
                if(dataReady){
                    FontRenderer::generateDataToCenter(segment, "Click to begin.", {0.f, 0.f}, 0.15f, {1.f, .86f, .55f, 1.f});
                    FontRenderer::generateDataToCenter(segment, "Controls: Arrow keys or WASD.", {0.f, -0.4f}, 0.1f, {1.f, .86f, .55f, 1.f});
                    FontRenderer::generateDataToCenter(segment, "Space/W/Up to jump.", {0.f, -0.5f}, 0.1f, {1.f, .86f, .55f, 1.f});
                    FontRenderer::generateDataToCenter(segment, "R to restart.", {0.f, -0.6f}, 0.1f, {1.f, .86f, .55f, 1.f});
                }else{
                    FontRenderer::generateDataToCenter(segment, "Loading required data...", {0.f, 0.f}, 0.15f, {1.f, .86f, .55f, 1.f});
                }
                window.setTexture(*font);
                window.draw(segment);
            }
            return;
        }
        double dt;
        double now = window.time->get();
        dt = now - lastUpdate;
        lastUpdate = now;
        if(dt > 0.1) dt = 0.1;
        std::visit([&](auto& state){ run(window, dt, state); }, programState);
        waitingJump = false;
    }
};
Program::Program(Window& window):data(new Impl(window)){}
Program::~Program() = default;
void Program::frame(Window& window){ data->frame(window); }
