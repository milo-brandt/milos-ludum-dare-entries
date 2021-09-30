//
//  GameState.hpp
//  Dummy
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include "Physics.h"
#include "ImmediateData.h"
#include "../Common/Window.h"
#include "Random.hpp"
#include <unordered_map>
#include <variant>

struct LoveData{
    Vector2<double> pos;
    double lifeLeft;
    double totalLife;
    double angleoffset;
    double anglespeed;
    double size;
    Vector2<double> offset;
    LoveData(Vector2<double> where):pos(where + getRandomNormal2()*0.08),lifeLeft(getRandomUniform(0.5, 2)),totalLife(lifeLeft),angleoffset(getRandomUniform(0, 6.28)),anglespeed(getRandomUniform(4, 12)),size(getRandomUniform(0.01, 0.1)),offset(getRandomNormal2()*0.5 - Vector2<double>{0.5, 0.5}){}
    bool update(RenderingSegment& segment, double dt){
        lifeLeft -= dt;
        if(lifeLeft < 0) return true;
        segment.addRotatedRectangle(pos, std::polar((float)size, (float)(angleoffset + lifeLeft*anglespeed)), Vector2<float>(offset) + standardTextureRect, standardTextureRect, {1.f, 0.f, 0.f, (float)(lifeLeft / totalLife)});
        return false;
    }
};
struct InputInfo{
    bool leftDown, rightDown, upDown, downDown;
    bool jumped;
};
struct DebouncedBool{
    double debounce;
    bool needed = false;
    double locked = 0;
    bool test(double when){
        bool ret = when > locked && needed;
        needed = false;
        if(ret) locked = when + debounce;
        return ret;
    }
};
class GameState;
namespace EscortActions{
    struct Kill{
        void act(GameState&, double dt, Escort* escort);
    };
    struct Safety{
        void act(GameState&, double dt, Escort* escort);
    };
    struct Accelerate{
        Vector2<double> acceleration;
        void act(GameState&, double dt, Escort* escort){
            escort->velocity += dt * acceleration;
        }
    };
    struct Propel{
        Vector2<double> target;
        double timeConstant; //Absolutely no more than 10.
        void act(GameState&, double dt, Escort* escort){
            escort->velocity += (target - escort->velocity) * timeConstant * dt;
        }
    };
    struct Special{
        std::function<void(GameState&, double dt, Escort* escort)> func;
        void act(GameState&, double dt, Escort* escort);
    };
    using Action = std::variant<Kill, Safety, Accelerate, Propel, Special>;
};
using EscortAction = EscortActions::Action;
namespace PlayerActions{
    struct Kill{
        void act(GameState&, double dt, Player* player);
    };
    struct Safety{
        void act(GameState&, double dt, Player* escort);
    };
    struct Special{
        std::function<void(GameState&, double dt, Player* escort)> func;
        void act(GameState&, double dt, Player* escort);
    };
    using Action = std::variant<Kill, Safety, Special>;
};
using PlayerAction = PlayerActions::Action;
namespace SensorRenderers{
    struct RenderData{
        RenderingSegment hearts;
        RenderingSegment lhearts;
        RenderingSegment rhearts;
        RenderingSegment white;
        void render(Window&, ImmediateData&);
    };
    struct Heart{
        Vector2<double> pos;
        double time;
        double size;
        double totalLife;
    };
    struct Love{
        struct Gen{
            RegionSensor* sensor;
            int num;
            std::vector<Heart> hearts;
            void render(RenderData&, double dt);
        };
        int num = 10;
        Gen generate(RegionSensor*) const;
    };
    struct BrokenLove{
        struct Gen{
            RegionSensor* sensor;
            int num;
            std::vector<Heart> rhearts;
            std::vector<Heart> lhearts;
            void render(RenderData&, double dt);
        };
        int num = 5;
        Gen generate(RegionSensor*) const;
    };
    struct SwitchGen{ //Always directly emplaced!
        RegionSensor* sensor;
        SharedDynamic<bool> on;
        Rectangle<double> base;
        void render(RenderData&, double dt);
    };
    struct Ghost{
        struct Gen{
            RegionSensor* sensor;
            double time = 0;
            void render(RenderData&, double dt);
        };
        Gen generate(RegionSensor*) const;
    };
    using Renderer = std::variant<Love, BrokenLove, Ghost>;
    using RendererGen = std::variant<Love::Gen, BrokenLove::Gen, SwitchGen, Ghost::Gen>;
};
using SensorRenderer = SensorRenderers::Renderer;
using SensorRendererGen = SensorRenderers::RendererGen;

struct RegistrationInfo{
    std::unordered_multimap<RegionSensor*, EscortAction> escortActions;
    std::unordered_multimap<RegionSensor*, PlayerAction> playerActions;
    std::vector<SensorRendererGen> renderers;
};
struct LevelDescription;
enum struct GameStatus{
    NORMAL,
    PLAYER_DIED,
    CIRCLE_DIED,
    VICTORY
};
class GameState{
public:
    std::vector<std::function<void(Window&, ImmediateData&)> > waitingFuncs;
    double hortTimer = 0;
    double lastDt = 0;
    struct RenderNotes{ double dt; };
    bool circleSafe, playerSafe;
    GameStatus status = GameStatus::NORMAL;
    Physics physics;
    RegistrationInfo registration;
    DebouncedBool needBounceSound{0.1};
    DebouncedBool needJumpSound{0.1};
    std::vector<LoveData> love;
    GameState();
    RegionSensor* addRegion(Rectangle<double> const& region){ physics.sensors.push_back(RegionSensor{region}); return physics.sensors.back().get(); }
    SolidObject* addBlock(Rectangle<double> const& region, bool square, bool circle){ physics.solids.push_back(SolidObject{region, square, circle}); return physics.solids.back().get(); }

    void loadLevel(LevelDescription const&);
    RenderNotes update(double dt, InputInfo const& info);
    void render(Window& window, ImmediateData& data, RenderNotes notes);
};

struct SensorDescription{
    Rectangle<double> region;
    std::vector<EscortAction> escortActions;
    std::vector<PlayerAction> playerActions;
    std::vector<SensorRenderer> renderers;
};
struct BlockDescription{
    Rectangle<double> region;
    bool square = true;
    bool circle = true;
};
struct PlayerDescription{
    Vector2<double> start;
};
struct EscortDescription{
    Vector2<double> start;
};
struct LevelDescription{
    std::vector<std::variant<SensorDescription, BlockDescription, PlayerDescription, EscortDescription> > items;
    std::function<void(GameState&)> additionalActions;
};

#endif /* GameState_hpp */
