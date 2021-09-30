/*
 *  line.h
 *  Acid
 *
 *  Created by Milo Brandt on 4/29/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */
#ifndef vector2_h
#define vector2_h
#include "vector2.h"
#define PI 3.141592653
struct circle{
	float x,y,r;
};
class line{
public:
	vector2 pointA, pointB;
	line(vector2 pointA = vector2(),vector2 pointB = vector2());
	line* splitBy(circle b); //returns 2 lines, or NULL if it doesn't work.
};
#endif