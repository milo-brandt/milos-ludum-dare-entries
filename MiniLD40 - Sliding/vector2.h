//
//  vector2.h
//  StructureSmash
//
//  Created by Milo Brandt on 12/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef StructureSmash_vector2_h
#define StructureSmash_vector2_h

class vector2{
public:
    float x,y;
    vector2(float x = 0,float y = 0);
    vector2 operator+(vector2 const& b) const;
    vector2 operator-(vector2 const& b) const;
    vector2 operator-() const;
    vector2 operator*(float const& b) const;
    vector2 operator/(float const& b) const;
    vector2& operator+=(vector2 const& b);
    vector2& operator-=(vector2 const& b);
    vector2& operator*=(float const& b);
    vector2& operator/=(float const& b);
    vector2 unit() const;
    vector2 rotL() const;
    vector2 rotR() const;
    float magnitude() const;
    float magnitudeSq() const;
    float dot(vector2 const& b) const;
    static vector2 smallestOnLine(vector2 const& a, vector2 const& b);
    static vector2 smallestOnLineSegment(vector2 const& a, vector2 const& b);
    static bool zeroNearLine(vector2 const& a, vector2 const& b);
};
vector2 operator*(float const& b,vector2 const& v);

#endif
