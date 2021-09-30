//
//  gameState.h
//  Mississippi10
//
//  Created by Milo Brandt on 8/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef Mississippi10_gameState_h
#define Mississippi10_gameState_h

#include <vector>
#include "ninja.h"
#include "rectangle.h"
class enemy;
class gameState{
public:
    gameState(int w,int h,int frames,float timeperframe,float nRad,vector2 gravity);
    int frames;
    int frame;
    float timeperframe;
    bool win;
    float nRad;
    vector2 gravity;
    std::vector<ninja> agents;
    std::vector<enemy*> enemies;
    ninja* selected;
    int w,h;
    std::vector<rectangle> staticRect;
    std::vector<rectangle> spikes;
    std::vector<rectangle*> movingRect;
    void update();
    void colCheck(ninja& n);
    bool colCheck(ninja& n,rectangle const& r);
    bool canJump(ninja& n);
    bool spiked(ninja& n);
    vector2 shurikenDemise(vector2 origin,vector2 target,int t);
    void fireShuriken(vector2 origin,vector2 target);
    float hitTime(vector2 origin,vector2 dir,rectangle r);
};

#endif
