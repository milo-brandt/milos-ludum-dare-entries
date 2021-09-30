//
//  matrix.cpp
//  StructureSmash
//
//  Created by Milo Brandt on 12/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "matrix.h"

const matrix matrix::identity(1,0,0,1);
matrix::matrix(float a,float b,float c,float d):a(a),b(b),c(c),d(d){}
vector2 matrix::operator*(const vector2& v) const{
    return vector2(a*v.x+b*v.y,c*v.x+d*v.y);
}
matrix matrix::operator*(const matrix &m) const{
    return matrix(a*m.a+b*m.c,a*m.b+b*m.d,
                  c*m.a+d*m.c,c*m.b+d*m.d);
}
matrix matrix::operator*(float const& v) const{
    return matrix(v*a,v*b,v*c,v*d);
}
matrix& matrix::operator*=(float const& s){
    a *= s;
    b *= s;
    c *= s;
    d *= s;
    return *this;
}
matrix operator*(float const& b,matrix const& m){
    return m*b;
}
matrix matrix::operator/(const float &v) const{
    return *this*(1/v);
}
matrix matrix::operator+(matrix const& v) const{
    return matrix(a+v.a,b+v.b,c+v.c,d+v.c);
}
matrix& matrix::operator+=(matrix const& v){
    a += v.a;
    b += v.b;
    c += v.c;
    d += v.d;
    return *this;
}
matrix matrix::operator-(matrix const& v) const{
    return matrix(a-v.a,b-v.b,c-v.c,d-v.c);
}
matrix matrix::operator-() const{
    return matrix(-a,-b,-c,-d);
}
matrix matrix::inverse() const{
    float det = a*d-b*c;
    return matrix(d/det,-b/det,-c/det,a/det);
}