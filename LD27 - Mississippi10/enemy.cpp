//
//  enemy.cpp
//  Mississippi10
//
//  Created by Milo Brandt on 8/25/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "enemy.h"
#include "gameState.h"

enemy::enemy(rectangle pos,int frames,bool critical):positions(new rectangle[frames]),critical(critical),killFrame(frames+1){
    for(int i = 0;i < frames;++i){
        positions[i]=pos;
    }
}
void enemy::onDeath(int frame, gameState *g){}