//
//  enemy.h
//  Mississippi10
//
//  Created by Milo Brandt on 8/25/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef Mississippi10_enemy_h
#define Mississippi10_enemy_h
#include "rectangle.h"

class gameState;
class enemy{
public:
    rectangle* positions;
    enemy(rectangle pos,int frames,bool critical = true);
    bool critical; //Must kill?
    int killFrame;
    virtual void onDeath(int frame,gameState* g);
};

#endif
