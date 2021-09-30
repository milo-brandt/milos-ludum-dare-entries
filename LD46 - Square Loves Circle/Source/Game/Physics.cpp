//
//  Physics.cpp
//  Dummy
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#include "Physics.h"
#include "../Templates/TemplateUtility.h"
#include <optional>

Vector2<double> distancesToRectangleFromPoint(Vector2<double> const& pt, Rectangle<double> const& rect){ //Shortest vector to rectangle
    double x, y;
    if(pt[0] < rect.lesser[0]) x = rect.lesser[0] - pt[0];
    else if(pt[0] < rect.greater[0]) x = 0;
    else x = rect.greater[0] - pt[0];
    if(pt[1] < rect.lesser[1]) y = rect.lesser[1] - pt[1];
    else if(pt[1] < rect.greater[1]) y = 0;
    else y = rect.greater[1] - pt[1];
    return {x,y};
}

struct Physics::Detail{
    static std::pair<double, double> getOverlapAmounts(Rectangle<double> const& player, Rectangle<double> const& object){
        double xOverlap = std::min(player.greater[0], object.greater[0]) - std::max(player.lesser[0], object.lesser[0]);
        double yOverlap = std::min(player.greater[1], object.greater[1]) - std::max(player.lesser[1], object.lesser[1]);
        return {xOverlap, yOverlap};
    }
    struct XOverlap{ double correctionAmt; };
    struct YOverlap{ double correctionAmt; };
    struct NoOverlap{};
    static std::variant<XOverlap, YOverlap, NoOverlap> handleCollision(Rectangle<double> const& player, Rectangle<double> const& object){
        auto [xOverlap, yOverlap] = getOverlapAmounts(player, object);
        if(xOverlap > 0 && yOverlap > 0){
            if(std::abs(xOverlap) < std::abs(yOverlap)){
                double candidate1 = object.lesser[0] - player.greater[0]; //Negative (Greater of player goes to lesser of object)
                double candidate2 = object.greater[0] - player.lesser[0]; //Positive (Lesser of player goes to greater of object
                if(std::abs(candidate1) < std::abs(candidate2)) return XOverlap{candidate1};
                else return XOverlap{candidate2};
            }else{
                double candidate1 = object.lesser[1] - player.greater[1]; //Negative (Greater of player goes to lesser of object)
                double candidate2 = object.greater[1] - player.lesser[1]; //Positive (Lesser of player goes to greater of object
                if(std::abs(candidate1) < std::abs(candidate2)) return YOverlap{candidate1};
                else return YOverlap{candidate2};
            }
        }else{
            return NoOverlap{};
        }
    }
    static bool rectanglesCollided(Rectangle<double> const& player, Rectangle<double> const& object){
        auto [xOverlap, yOverlap] = getOverlapAmounts(player, object);
        return xOverlap > 0 && yOverlap > 0;
    }
    
    struct EventList{
        std::vector<PlayerCollidedWithObject> playerCollidedWithObject;
        std::vector<PlayerSensed> playerSensed;
        std::vector<PlayerCollidedWithEscort> playerCollidedWithEscort;
        std::vector<EscortCollidedWithObject> escortCollidedWithObject;
        std::vector<EscortSensed> escortSensed;
    };
    struct CorrectionInfo{ double amt; SolidObject* object; };
    static void updatePlayer(Physics& me, EventList& events, double dt, Player& player){
        player.center += player.velocity * dt;
        player.constraint = {Constraint::NONE, Constraint::NONE};
        for(int i = 0; i < 2; ++i){
            Rectangle<double> playerRect = player.center + player.hitbox;
            std::optional<CorrectionInfo> xCorrection;
            std::optional<CorrectionInfo> yCorrection;
            for(auto& object : me.solids){
                if(!object->canHitSquare) continue;
                std::visit(overloaded{
                    [&](XOverlap overlap){
                        if(!xCorrection || std::abs(xCorrection->amt) > std::abs(overlap.correctionAmt)) xCorrection = CorrectionInfo{overlap.correctionAmt, object.get()};
                    },
                    [&](YOverlap overlap){
                        if(!yCorrection || std::abs(yCorrection->amt) > std::abs(overlap.correctionAmt)) yCorrection = CorrectionInfo{overlap.correctionAmt, object.get()};
                    },
                    [](NoOverlap){}
                }, handleCollision(playerRect, object->region));
            }
            if(xCorrection && yCorrection){
                if(std::abs(xCorrection->amt) < std::abs(yCorrection->amt)) xCorrection.reset();
                else yCorrection.reset();
            }
            if(xCorrection){
                player.center[0] += xCorrection->amt;
                if(std::abs(xCorrection->amt) > 0.0001) player.velocity[0] = 0;
                events.playerCollidedWithObject.push_back({xCorrection->object, &player});
                if(xCorrection->amt > 0) player.constraint[0] = Constraint::NEG;
                else player.constraint[0] = Constraint::POS;
            }
            if(yCorrection){
                player.center[1] += yCorrection->amt;
                if(std::abs(yCorrection->amt) > 0.0001) player.velocity[1] = 0;
                events.playerCollidedWithObject.push_back({yCorrection->object, &player});
                if(yCorrection->amt > 0) player.constraint[1] = Constraint::NEG;
                else player.constraint[1] = Constraint::POS;
            }
        }
        auto playerRect = player.center + player.hitbox;
        auto expanded = playerRect.expand(0.005);
        if(player.constraint[0] == Constraint::NONE){ //Check for anything *close* - for wall jumping
            for(auto& object : me.solids){
                if(!object->canHitSquare) continue;
                std::visit(overloaded{
                    [&](XOverlap overlap){
                        if(overlap.correctionAmt > 0) player.constraint[0] = Constraint::NEG;
                        else player.constraint[0] = Constraint::POS;
                    },
                    [](YOverlap overlap){},
                    [](NoOverlap){}
                }, handleCollision(expanded, object->region));
            }
        }
        for(auto& sensor : me.sensors){
            if(rectanglesCollided(playerRect, sensor->region)){
                events.playerSensed.push_back({sensor.get(), &player});
            }
        }
    }

    static bool circleCollidedWithRect(Vector2<double> const& pt, Rectangle<double> const& rect, double rad){
        auto vec = distancesToRectangleFromPoint(pt, rect);
        return vec.magnitudeSq() < rad*rad;
    }
    //Returns required offset
    static std::optional<Vector2<double> > handleCircleRectCollision(Vector2<double> const& pt, Rectangle<double> const& rect, double rad){
        auto vec = distancesToRectangleFromPoint(pt, rect);
        if(vec.magnitudeSq() < rad*rad){
            if(vec.magnitudeSq() < 0.001){ //Inside rect! Oh no.
                double distances[4] = {pt[0] - rect.lesser[0], rect.greater[0] - pt[0], pt[1] - rect.lesser[1], rect.greater[1] - pt[1]};
                std::size_t mindex = 0;
                if(distances[1] < distances[0]) mindex = 1;
                if(distances[2] < distances[mindex]) mindex = 2;
                if(distances[3] < distances[mindex]) mindex = 3;
                if(mindex == 0) return Vector2<double>{rect.lesser[0] - rad - pt[0], 0}; //Find shortest path out.
                if(mindex == 1) return Vector2<double>{rect.greater[0] + rad - pt[0], 0};
                if(mindex == 2) return Vector2<double>{0, rect.lesser[1] - rad - pt[1]};
                if(mindex == 3) return Vector2<double>{0, rect.greater[1] + rad - pt[1]};
            }else{ //Outside - follow normal outward
                double dist = vec.magnitude();
                auto ret = vec * (1 - rad / dist);
                return ret;
            }
        }
        return std::nullopt;
    }
    static void updateEscort(Physics& me, EventList& events, double dt, Escort& escort){
        escort.position += escort.velocity * dt;

        std::vector<Player*> possibleCollisionPlayers;
        std::vector<SolidObject*> possibleCollisionObjects;
        for(auto& object : me.players){
            auto correction = handleCircleRectCollision(escort.position, object->center + object->hitbox, escort.radius);
            if(correction){
                escort.position += *correction;
                if(correction->magnitudeSq() > 0.000000001){
                    auto badVelocity = correction->parallelComponentOf(escort.velocity);
                    if(badVelocity.dot(*correction) < 0){ //Avoid case where things are already handled!
                        escort.velocity -= badVelocity * 1.5; //0.5 bounce factor
                    }
                }
                possibleCollisionPlayers.push_back(object.get());
            }
        }
        for(auto& object : me.solids){
            if(!object->canHitCircle) continue;
            auto correction = handleCircleRectCollision(escort.position, object->region, escort.radius);
            if(correction){
                escort.position += *correction;
                if(correction->magnitudeSq() > 0.000000001){
                    auto badVelocity = correction->parallelComponentOf(escort.velocity);
                    if(badVelocity.dot(*correction) < 0){ //Avoid case where things are already handled!
                        escort.velocity -= badVelocity * 1.5; //0.5 bounce factor
                    }
                }
                possibleCollisionObjects.push_back(object.get());
            }
        }
        //Push back on the player if needed!
        for(auto& object : me.players){
            auto correction = handleCircleRectCollision(escort.position, object->center + object->hitbox, escort.radius);
            if(correction){
                object->center -= *correction;
                if(correction->magnitudeSq() > 0.000000001) object->velocity = correction->perpendicularComponentOf(object->velocity);
                possibleCollisionPlayers.push_back(object.get());
            }
        }
        //Now check which collisions were real.
        double cutoff = escort.radius * escort.radius + 0.01;
        for(auto object : possibleCollisionPlayers){
            auto dist = distancesToRectangleFromPoint(escort.position, object->center + object->hitbox);
            if(dist.magnitudeSq() < cutoff){
                events.playerCollidedWithEscort.push_back({object, &escort});
                /*Update constraints on player*/
                if(std::abs(dist[0]) > std::abs(dist[1])){
                    if(dist[0] > 0) object->constraint[0] = Constraint::NEG;
                    else object->constraint[0] = Constraint::POS;
                }else{
                    if(dist[1] > 0) object->constraint[1] = Constraint::NEG;
                    else object->constraint[1] = Constraint::POS;
                }
            }
        }
        for(auto object : possibleCollisionObjects){
            auto dist = distancesToRectangleFromPoint(escort.position, object->region);
            if(dist.magnitudeSq() < cutoff){
                events.escortCollidedWithObject.push_back({object, &escort});
            }
        }
        //Finally check sensors
        for(auto& object : me.sensors){
            if(circleCollidedWithRect(escort.position, object->region, escort.radius)){
                events.escortSensed.push_back({object.get(), &escort});
            }
        }
    }
};
void Physics::update(double dt){
    Detail::EventList events;
    for(auto& player : players){ //Probably only one player
        Detail::updatePlayer(*this, events, dt, *player);
    }
    for(auto& escort : escorts){ //Probably only one player
        Detail::updateEscort(*this, events, dt, *escort);
    }

    for(auto& event : events.playerCollidedWithObject){
        playerCollidedWithObject.fire(event);
    }
    for(auto& event : events.playerSensed){
        playerSensed.fire(event);
    }
    for(auto& event : events.playerCollidedWithEscort){
        playerCollidedWithEscort.fire(event);
    }
    for(auto& event : events.escortCollidedWithObject){
        escortCollidedWithObject.fire(event);
    }
    for(auto& event : events.escortSensed){
        escortSensed.fire(event);
    }
}

