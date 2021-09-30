/*
 *  line.cpp
 *  Acid
 *
 *  Created by Milo Brandt on 4/29/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */

#include "line.h"
#include <math.h>
line::line(vector2 pointA,vector2 pointB):pointA(pointA),pointB(pointB){
}
line* line::splitBy(circle b){
	float m = (pointA.y - pointB.y)/(pointA.x - pointB.x);
	float q = pointA.y - (m * (pointA.x - b.x)) - b.y;
	float top = -q*q+m*m*b.r*b.r+b.r*b.r;
	if(top < 0) return 0;
	top = sqrt(top);
	float xA = (-top-q*m)/(m*m+1);
	float xB = (top-q*m)/(m*m+1);
	line* r = new line[2];
	if(xA+b.x < pointA.x){
		if(xB+b.x < pointA.x) return 0;
		if(xB+b.x > pointB.x){
			r[0].pointA = vector2(1000,1000);
			return r;
		}
		r[0].pointB = pointB;
		r[0].pointA.x = xB+b.x;
		r[0].pointA.y = m*xB+q+b.y;
		r[1].pointA = vector2(1000,1000);
		return r;
	}
	if(xB+b.x > pointB.x){
		if(xA+b.x > pointB.x) return 0;
		r[0].pointA = pointA;
		r[0].pointB.x = xA+b.x;
		r[0].pointB.y = m*xA+q+b.y;
		r[1].pointA = vector2(1000,1000);
		return r;
	}
	r[0].pointA = pointA;
	r[0].pointB.x = xA+b.x;
	r[0].pointB.y = m*xA+q+b.y;
	r[1].pointB = pointB;
	r[1].pointA.x = xB+b.x;
	r[1].pointA.y = m*xB+q+b.y;
	return r;
}