//
//  player.h
//  StructureSmash
//
//  Created by Milo Brandt on 2/22/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef StructureSmash_player_h
#define StructureSmash_player_h

#include "vector2.h"
#include "grid.h"

class gameInstance;
class player{
public:
    vector2 pos; //feet
    vector2 veloc;
    int underx,undery;
    bool wantJump;
    float wantMove;
    player();
    bool collidesWith(gameInstance*,int x,int y);
    vector2 correctCollision(gameInstance*,int x,int y);
    void move(gameInstance*,float dt);
    vector2 correctOneCollision(gameInstance*);
};

#endif
