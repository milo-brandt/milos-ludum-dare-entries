//
//  ninja.cpp
//  Mississippi10
//
//  Created by Milo Brandt on 8/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ninja.h"

ninja::ninja(vector2 const& pos1,vector2 const& veloc1,int const& frames):pos(new vector2[frames]),veloc(new vector2[frames]),frameSize(frames),movAction(new char[frameSize]),deathTime(frameSize),shurikenTime(-1){
    pos[0] = pos1;
    veloc[0] = veloc1;
    for(int i = 0;i < frameSize;++i){
        movAction[i] = '.';
    }
}