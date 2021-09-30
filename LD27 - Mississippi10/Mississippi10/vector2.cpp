//
//  vector2.cpp
//  Mississippi10
//
//  Created by Milo Brandt on 8/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

//
//  vector2.cpp
//  StructureSmash
//
//  Created by Milo Brandt on 12/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <math.h>
#include "vector2.h"

vector2::vector2(float x,float y):x(x),y(y){}
vector2 vector2::operator+(vector2 const& b) const{
    return vector2(x+b.x,y+b.y);
}
vector2& vector2::operator+=(vector2 const& b){
    x += b.x;
    y += b.y;
    return *this;
}
vector2& vector2::operator*=(float const& b){
    x *= b;
    y *= b;
    return *this;
}
vector2& vector2::operator-=(vector2 const& b){
    x -= b.x;
    y -= b.y;
    return *this;
}
vector2& vector2::operator/=(float const& b){
    x /= b;
    y /= b;
    return *this;
}

vector2 vector2::operator-(vector2 const& b) const{
    return vector2(x-b.x,y-b.y);
}
vector2 vector2::operator-() const{
    return vector2(-x,-y);
}
vector2 vector2::operator*(float const& b) const{
    return vector2(x*b,y*b);
}
vector2 vector2::operator/(float const& b) const{
    return vector2(x/b,y/b);
}
vector2 operator*(float const& b,vector2 const& v){
    return vector2(v.x*b,v.y*b);
}
float vector2::magnitude() const{
    return sqrt(magnitudeSq());
}
float vector2::magnitudeSq() const{
    return x*x+y*y;
}
vector2 vector2::unit() const{
    return *this/magnitude();
}
float vector2::dot(const vector2 &b) const{
    return x*b.x+y*b.y;
}
vector2 vector2::rotL() const{
    return vector2(y,-x);
}
vector2 vector2::rotR() const{
    return vector2(-y,x);
}
vector2 vector2::smallestOnLine(vector2 const& a, vector2 const& b){
    vector2 norm = (a-b).rotL();
    return norm*(norm.dot(a)/norm.magnitudeSq());
}
vector2 vector2::smallestOnLineSegment(vector2 const& a, vector2 const& b){
    if(zeroNearLine(a, b)) return smallestOnLine(a, b);
    return (a.magnitudeSq()<b.magnitudeSq())?a:b;
}
bool vector2::zeroNearLine(vector2 const& a, vector2 const& b){
    vector2 dif = a-b;
    if(dif.magnitudeSq() < 0.0001) return false;
    return signbit(dif.dot(a)) != signbit(dif.dot(b));
}