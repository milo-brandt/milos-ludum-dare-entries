//
//  Physics.hpp
//  Dummy
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef Physics_hpp
#define Physics_hpp

#include "../Templates/Event.h"
#include "../Templates/Shapes.h"
#include "../Templates/IndirectValue.h"
#include <variant>
#include <vector>
#include <functional>

Vector2<double> distancesToRectangleFromPoint(Vector2<double> const& pt, Rectangle<double> const& rect);
struct SolidObject{
    Rectangle<double> region;
    bool canHitSquare = true;
    bool canHitCircle = true;
};
struct RegionSensor{
    Rectangle<double> region;
};
enum struct Constraint{
    NONE, POS, NEG
}; //POS = cannot go more positive
template<class T>
void constrainBy(Constraint constraint, T& value){
    if(constraint == Constraint::POS){
        if(value > 0) value = 0;
    }else if(constraint == Constraint::NEG){
        if(value < 0) value = 0;
    }
}
template<class T>
T constrainByRet(Constraint constraint, T value){
    constrainBy(constraint, value);
    return value;
}
struct Player{
    Rectangle<double> hitbox;
    Vector2<double> center;
    Vector2<double> velocity;
    Vector2<Constraint> constraint = {Constraint::NONE, Constraint::NONE};
};
struct Escort{
    double radius;
    Vector2<double> position;
    Vector2<double> velocity;
};
struct PlayerCollidedWithObject{
    SolidObject* object;
    Player* player;
};
struct PlayerSensed{
    RegionSensor* sensor;
    Player* player;
};
struct PlayerCollidedWithEscort{
    Player* player;
    Escort* escort;
};
struct EscortCollidedWithObject{
    SolidObject* object;
    Escort* player;
};
struct EscortSensed{
    RegionSensor* sensor;
    Escort* player;
};

class Physics{
    struct Detail;
public:
    std::vector<IndirectValue<SolidObject> > solids;
    std::vector<IndirectValue<RegionSensor> > sensors;
    std::vector<IndirectValue<Escort> > escorts;
    std::vector<IndirectValue<Player> > players;
    Event<PlayerCollidedWithObject> playerCollidedWithObject;
    Event<PlayerSensed> playerSensed;
    Event<PlayerCollidedWithEscort> playerCollidedWithEscort;
    Event<EscortCollidedWithObject> escortCollidedWithObject;
    Event<EscortSensed> escortSensed;
    void update(double dt);
};

#endif /* Physics_hpp */
