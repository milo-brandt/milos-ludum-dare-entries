//
//  GameState.cpp
//  Dummy
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#include "GameState.hpp"

namespace{
    void bounce(GameState& state, PlayerCollidedWithEscort collision){
        auto vec = distancesToRectangleFromPoint(collision.escort->position, collision.player->center + collision.player->hitbox);
        collision.escort->velocity = collision.player->velocity + vec.normalize(-0.25);
        for(int i = 0; i < 5; ++i){
            state.love.emplace_back(collision.escort->position + vec);
        }
    }
}
namespace{
    void incorporateItem(GameState& me, SensorDescription const& desc){
        me.physics.sensors.push_back(RegionSensor{desc.region});
        auto back = me.physics.sensors.back().get();
        for(auto const& action : desc.escortActions){
            me.registration.escortActions.insert(std::make_pair(back, action));
        }
        for(auto const& action : desc.playerActions){
            me.registration.playerActions.insert(std::make_pair(back, action));
        }
        for(auto const& renderer : desc.renderers){
            me.registration.renderers.push_back(std::visit([&](auto const& spec) -> SensorRendererGen{
                return spec.generate(back);
            }, renderer));
        }
    }
    void incorporateItem(GameState& me, BlockDescription const& desc){
        me.physics.solids.push_back(SolidObject{desc.region, desc.square, desc.circle});
    }
    void incorporateItem(GameState& me, PlayerDescription const& desc){
        me.physics.players.push_back(Player{
            {{-0.05, -0.05}, {0.05, 0.05}}, //Hitbox.
            desc.start
        });
    }
    void incorporateItem(GameState& me, EscortDescription const& desc){
        me.physics.escorts.push_back(Escort{
            0.1, // radius;
            desc.start
        });
    }

}
void GameState::loadLevel(LevelDescription const& description){
    physics.escorts.clear();
    physics.sensors.clear();
    physics.solids.clear();
    physics.players.clear();
    registration.escortActions.clear();
    registration.playerActions.clear();
    registration.renderers.clear();
    for(auto const& item : description.items){
        std::visit([&](auto const& item){ incorporateItem(*this, item); }, item);
    }
    if(description.additionalActions) description.additionalActions(*this);
}
GameState::GameState(){
    physics.playerCollidedWithEscort.hook([this](PlayerCollidedWithEscort collision){
        bounce(*this, collision);
        needBounceSound.needed = true;
    });
    physics.playerSensed.hook([this](PlayerSensed sensed){
        auto [pos, end] = registration.playerActions.equal_range(sensed.sensor);
        for(; pos != end; ++pos){
            std::visit([&](auto& action){
                action.act(*this, lastDt, sensed.player);
            }, pos->second);
        }
    });
    physics.escortSensed.hook([this](EscortSensed sensed){
        auto [pos, end] = registration.escortActions.equal_range(sensed.sensor);
        for(; pos != end; ++pos){
            std::visit([&](auto& action){
                action.act(*this, lastDt, sensed.player);
            }, pos->second);
        }
    });
}
GameState::RenderNotes GameState::update(double dt, InputInfo const& info){
    RenderNotes ret;
    ret.dt = dt;

    if(status != GameStatus::NORMAL){
        if(status == GameStatus::VICTORY){
            hortTimer += dt;
            while(hortTimer > 0.05){
                hortTimer -= 0.05;
                love.emplace_back(getRandomInRectangle({{-1.4, -1}, {1.4, 1}}));
            }
        }
        return ret;
    }
    lastDt = dt;
    circleSafe = false;
    playerSafe = false;
    Player* player = physics.players.empty() ? nullptr : physics.players[0].get();
    if(player){
        Vector2<double> acceleration;
        acceleration = {0, -2};
        double playerLateralAbility = player->constraint[1] == Constraint::NEG ? 1.5 : 0.33;
        player->velocity *= (1 - dt);
        if(info.leftDown) acceleration += Vector2<double>{-playerLateralAbility, 0};
        if(info.rightDown) acceleration += Vector2<double>{playerLateralAbility, 0};
        if(info.upDown) acceleration += Vector2<double>{0, 0.75};
        if(info.downDown) acceleration -= Vector2<double>{0, 1};
        player->velocity += acceleration * dt;
        if(player->constraint[0] != Constraint::NONE){ //Wall jump!
            player->velocity[1] *= (1 - 2 * dt); //Slide!
        }
        if(info.jumped){
            if(player->constraint[1] == Constraint::NEG){
                player->velocity[1] = 1 + std::min(std::abs(player->velocity[0])*0.5, 0.5);
                needJumpSound.needed = true;
            }else{
                if(player->constraint[0] != Constraint::NONE){ //Wall jump!
                    player->velocity[0] = player->constraint[0] == Constraint::POS ? -0.5 : 0.5;
                    player->velocity[1] = 1;
                    needJumpSound.needed = true;
                }
            }
        }
    }
    physics.update(dt);
    if(playerSafe && circleSafe){
        status = GameStatus::VICTORY;
    }
    return ret;
}
void GameState::render(Window& window, ImmediateData& data, RenderNotes notes){
    double now = window.time->get();
    for(auto& f : waitingFuncs) f(window, data);
    waitingFuncs.clear();
    if(needJumpSound.test(now)) window.playSample(data.jump, 0.1f);
    if(needBounceSound.test(now)) window.playSample(data.bounce, 0.1f);
    RenderingSegment simpleView;
    for(auto const& block : physics.solids){
        if(!block->canHitCircle && !block->canHitSquare) continue; //Block doesn't exist.
        Vector<float, 4> color = block->canHitCircle ? block->canHitSquare ? Vector<float, 4>{0.52f, 0.27f, 0.01f, 1.f} : Vector<float, 4>{0.52f, 0.27f, 0.01f, 0.5f} : Vector<float, 4>{0.52f, 0.27f, 0.6f, 0.5f};
        simpleView.addRectangle(block->region, standardTextureRect, color);
    }
    for(auto const& player : physics.players){
        simpleView.addRectangle(player->center + player->hitbox, standardTextureRect, {1.f, .86f, .55f, 1.f});
    }
    /*for(auto const& sensor : physics.sensors){
        simpleView.addRectangle(sensor->region, standardTextureRect, {0.75f, 0.75f, 0.f, 0.1f});
    }*/
    RenderingSegment escorts;
    for(auto const& escort : physics.escorts){
        escorts.addRotatedRectangle(escort->position, std::polar((float)escort->radius, (float)(-escort->position[0] / escort->radius)), Rectangle<float>::squareFromCenter({}, 1.f), standardTextureRect, {1.f, .86f, .55f, 1.f});
    }
    RenderingSegment loves;
    eraseFromVectorIf(love, [&loves, dt = notes.dt](LoveData& data){
        return data.update(loves, dt);
    });
    
    window.clear({0.18f, 0.08f, 0.01f});
    window.setUniforms({{0.f, 0.f}, {1 / window.getAspectRatio(), 1.f}});
    SensorRenderers::RenderData sensorRendering;
    for(auto& renderer : registration.renderers){
        std::visit([&](auto& renderer){ renderer.render(sensorRendering, notes.dt); }, renderer);
    }
    sensorRendering.render(window, data);
    window.setTexture(data.white);
    window.draw(simpleView);
    window.setTexture(data.circle);
    window.draw(escorts);
    window.setTexture(data.love);
    window.draw(loves);
}
namespace EscortActions{
    void Kill::act(GameState& state, double dt, Escort* escort){
        state.status = GameStatus::CIRCLE_DIED;
    }
    void Safety::act(GameState& state, double dt, Escort* escort){
        state.circleSafe = true;
    }
    void Special::act(GameState& state, double dt, Escort* escort){
        func(state, dt, escort);
    }

}
namespace PlayerActions{
    void Kill::act(GameState& state, double dt, Player* player){
        state.status = GameStatus::PLAYER_DIED;
    }
    void Safety::act(GameState& state, double dt, Player* player){
        state.playerSafe = true;
    }
    void Special::act(GameState& state, double dt, Player* player){
        func(state, dt, player);
    }
}
namespace SensorRenderers{
    namespace{
        Heart makeHeart(Rectangle<double> const& region){
            Heart ret;
            ret.pos = getRandomInRectangle(region);
            ret.time = 0;
            ret.size = getRandomUniform(0.01, 0.1);
            ret.totalLife = getRandomUniform(0.5, 4);
            return ret;
        }
        bool runHeart(Heart& heart, RenderingSegment& segment, double dt, Vector<float, 4> const& color = {1.f, 1.f, 1.f, 1.f}){
            heart.time += dt;
            if(heart.time > heart.totalLife) return true;
            double alpha = 0.5 - std::abs(0.5 - heart.time / heart.totalLife);
            segment.addRectangle(Rectangle<float>::squareFromCenter(heart.pos, heart.size / 2), oppositeTextureRect, {color[0], color[1], color[2], color[3] * (float)alpha});
            return false;
        }
    }
    Love::Gen Love::generate(RegionSensor* sensor) const{
        //max hearts : 10
        Love::Gen ret;
        ret.sensor = sensor;
        ret.num = num;
        for(int i = 0; i < num; ++i){
            Heart heart = makeHeart(sensor->region);
            heart.totalLife = getRandomUniform(0, heart.totalLife);
            ret.hearts.push_back(heart);
        }
        return ret;
    }
    void Love::Gen::render(RenderData &data, double dt){
        eraseFromVectorIf(hearts, [&](Heart& heart){
            return runHeart(heart, data.hearts, dt);
        });
        while(hearts.size() < num){
            hearts.push_back(makeHeart(sensor->region));
        }
    }
    BrokenLove::Gen BrokenLove::generate(RegionSensor* sensor) const{
        //max hearts : 10
        BrokenLove::Gen ret;
        ret.sensor = sensor;
        ret.num = num;
        for(int i = 0; i < num; ++i){
            Heart heart = makeHeart(sensor->region);
            heart.totalLife = getRandomUniform(0, heart.totalLife);
            ret.lhearts.push_back(heart);
        }
        for(int i = 0; i < num; ++i){
            Heart heart = makeHeart(sensor->region);
            heart.totalLife = getRandomUniform(0, heart.totalLife);
            ret.rhearts.push_back(heart);
        }
        return ret;
    }
    void BrokenLove::Gen::render(RenderData &data, double dt){
        eraseFromVectorIf(lhearts, [&](Heart& heart){
            return runHeart(heart, data.lhearts, dt, {0.f, 0.f, 0.f, 2.f});
        });
        eraseFromVectorIf(rhearts, [&](Heart& heart){
            return runHeart(heart, data.rhearts, dt, {0.f, 0.f, 0.f, 2.f});
        });
        while(lhearts.size() < num){
            lhearts.push_back(makeHeart(sensor->region));
        }
        while(rhearts.size() < num){
            rhearts.push_back(makeHeart(sensor->region));
        }
    }
    Ghost::Gen Ghost::generate(RegionSensor* sensor) const{
        return {sensor};
    }
    void Ghost::Gen::render(RenderData& data, double dt){
        time += dt;
        float alpha = float((std::sin(time * 9) + 2) / 5);
        data.white.addRectangle(sensor->region, standardTextureRect, {0.33f, 0.33f, 0.44f, alpha});
    }
    void SwitchGen::render(RenderData& data, double dt){
        //Rectangle<double> buttonRegion = on->get() ? sensor->region : Rectangle<double>{sensor->region.lesser * 0.5 + base.lesser * 0.5, sensor->region.greater * 0.5 + base.greater * 0.5};
        if(!on->get()) data.white.addRectangle(sensor->region, standardTextureRect, {1.f, 0.f, 0.f, 1.f});
        data.white.addRectangle(base, standardTextureRect, {0.55f, 0.45f, 0.6f, 1.f});
    }
    void RenderData::render(Window& window, ImmediateData& data){
        window.setTexture(data.love);
        window.draw(hearts);
        window.setTexture(data.brokenLeft);
        window.draw(lhearts);
        window.setTexture(data.brokenRight);
        window.draw(rhearts);
        window.setTexture(data.white);
        window.draw(white);
    }

}
