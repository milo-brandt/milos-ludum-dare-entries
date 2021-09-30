//
//  Shapes.hpp
//  TestApp
//
//  Created by Milo Brandt on 9/15/19.
//  Copyright © 2019 Milo Brandt. All rights reserved.
//

#ifndef Shapes_hpp
#define Shapes_hpp

#include "Vector.h"

template<class T>
struct Rectangle{
    Vector2<T> lesser;
    Vector2<T> greater;
    constexpr Rectangle() = default;
    constexpr Rectangle(Vector2<T> const& lesser,Vector2<T> const& greater):lesser(lesser),greater(greater){}
    bool contains(Vector2<T> const& me) const;
    bool contains(Rectangle<T> const& other) const;
    bool near(Vector2<T> const& me,T rad) const;
    T distanceTo(Vector2<T> const& pos) const;
    Vector2<T> size() const;
    static Rectangle fromCorners(Vector2<T> const& a,Vector2<T> const& b);
    static Rectangle squareFromCenter(Vector2<T> const&, T rad = T(0.5));
    template <class U, class = std::enable_if_t<std::is_convertible_v<T, U> > >
    operator Rectangle<U>() const{
        return {Vector2<U>(lesser), Vector2<U>(greater)};
    }
    Vector2<T> center() const{
        return (lesser + greater)/2;
    }
    Rectangle<T> extendTo(Vector2<T> point) const{
        Rectangle<T> ret = *this;
        if(point[0] < lesser[0]) ret.lesser[0] = point[0];
        if(point[1] < lesser[1]) ret.lesser[1] = point[1];
        if(point[0] > greater[0]) ret.greater[0] = point[0];
        if(point[1] > greater[1]) ret.greater[1] = point[1];
        return ret;
    }
    Rectangle<T> expand(T const& amt) const{
        return {lesser - Vector2<T>{amt, amt}, greater + Vector2<T>{amt, amt}};
    }
    Vector2<T> cornerLG() const{
        return {lesser[0], greater[1]};
    }
    Vector2<T> cornerGL() const{
        return {greater[0], lesser[1]};
    }
};
template<class T>
bool Rectangle<T>::contains(Vector2<T> const& me) const{
    return me[0] >= lesser[0] && me[1] >= lesser[1] && me[0] <= greater[0] && me[1] <= greater[1];
}
template<class T>
bool Rectangle<T>::contains(Rectangle<T> const& other) const{
    return lesser[0] <= other.lesser[0] && lesser[1] <= other.lesser[1] && greater[0] >= other.greater[0] && greater[1] >= other.greater[1];
}
template<class T>
bool Rectangle<T>::near(Vector2<T> const& me, T rad) const{
    return me[0] >= lesser[0] - rad && me[1] >= lesser[1] - rad && me[0] <= greater[0] + rad && me[1] <= greater[1] + rad;
}
template<class T>
T Rectangle<T>::distanceTo(Vector2<T> const& me) const{
    T horizontalDistance = me[0] < lesser[0] ? lesser[0] - me[0] : me[0] <= greater[0] ? 0 : me[0] - greater[0];
    T verticalDistance = me[1] < lesser[1] ? lesser[1] - me[1] : me[1] <= greater[1] ? 0 : me[1] - greater[1];
    return std::max(horizontalDistance, verticalDistance);
}
template<class T>
Vector2<T> Rectangle<T>::size() const{
    return greater - lesser;
}
template<class T>
Rectangle<T> Rectangle<T>::fromCorners(Vector2<T> const& a, Vector2<T> const& b){
    return {{std::min(a[0],b[0]),std::min(a[1],b[1])},{std::max(a[0],b[0]),std::max(a[1],b[1])}};
}
template<class T>
Rectangle<T> Rectangle<T>::squareFromCenter(Vector2<T> const& a, T len){
    Vector2<double> off{len,len};
    return {a-off,a+off};
}
inline Rectangle<double> boundsOfIntegerRect(Rectangle<int> rect){
    return {rect.lesser - Vector2<double>{0.5,0.5}, rect.greater + Vector2<double>{0.5,0.5}};
}
template<class Func>
void forEachPointInRectangle(Rectangle<int> rect, Func&& func){
    for(int x = rect.lesser[0]; x <= rect.greater[0]; ++x){
        for(int y = rect.lesser[1]; y <= rect.greater[1]; ++y){
            if constexpr(std::is_invocable_v<Func, Vector2<int> >){
                func(Vector2<int>{x,y});
            }else{
                static_assert(std::is_invocable_v<Func, int, int>);
                func(x,y);
            }
        }
    }
}
template<class T>
Rectangle<T> operator+(Rectangle<T> const& rect, Vector2<T> const& translate){
    return {rect.lesser + translate, rect.greater + translate};
}
template<class T>
Rectangle<T> operator+(Vector2<T> const& translate, Rectangle<T> const& rect){
    return {translate + rect.lesser, translate + rect.greater};
}

constexpr Rectangle<float> standardTextureRect{{0.f, 0.f}, {1.f, 1.f}};
constexpr Rectangle<float> oppositeTextureRect{{0.f, 1.f}, {1.f, 0.f}};

#endif /* Shapes_hpp */
