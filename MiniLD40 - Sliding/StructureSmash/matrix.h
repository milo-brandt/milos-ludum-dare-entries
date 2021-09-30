//
//  matrix.h
//  StructureSmash
//
//  Created by Milo Brandt on 12/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef StructureSmash_matrix_h
#define StructureSmash_matrix_h

#include "vector2.h"

class matrix{
public:
    float a,b,c,d;
    static const matrix identity;
    matrix(float a,float b,float c,float d);
    vector2 operator*(vector2 const& b) const;
    matrix operator*(matrix const& b) const;
    matrix operator*(float const& b) const;
    matrix operator/(float const& b) const;
    matrix operator+(matrix const& b) const;
    matrix& operator+=(matrix const& b);
    matrix& operator*=(float const& b);
    matrix operator-(matrix const& b) const;
    matrix operator-() const;
    matrix inverse() const;
};
matrix operator*(float const& b,matrix const& m);
#endif
