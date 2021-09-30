//
//  rectangle.h
//  Mississippi10
//
//  Created by Milo Brandt on 8/25/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef Mississippi10_rectangle_h
#define Mississippi10_rectangle_h
#include "vector2.h"

class rectangle{
public:
    vector2 upLeft;
    vector2 downRight;
    rectangle(vector2 upLeft = vector2(),vector2 downRight = vector2());
};

#endif
