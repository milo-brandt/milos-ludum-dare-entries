//
//  vector2.h
//  Mississippi10
//
//  Created by Milo Brandt on 8/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef Mississippi10_vector2_h
#define Mississippi10_vector2_h

//
//  vector2.h
//  StructureSmash
//
//  Created by Milo Brandt on 12/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.

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
