//
//  ninja.h
//  Mississippi10
//
//  Created by Milo Brandt on 8/23/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef Mississippi10_ninja_h
#define Mississippi10_ninja_h

#include <vector>
#include "vector2.h"

class ninja{ //A what-ja?
public:
    int frameSize;
    char* movAction;
    vector2* pos;
    vector2* veloc;
    int shurikenTime;
    int deathTime;
    vector2 shurikenTarg;
    ninja(vector2 const& pos1,vector2 const& veloc1,int const& frames);
};

#endif
