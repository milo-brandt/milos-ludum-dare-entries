//
//  IndirectValue.h
//  CAM
//
//  Created by Milo Brandt on 3/22/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef IndirectValue_h
#define IndirectValue_h

#include <memory>
#include <utility>

template<class T>
class IndirectValue{
    std::unique_ptr<T> value;
public:
    template<class... Args>
    IndirectValue(Args&&... args):value(new T(std::forward<Args>(args)...)){}
    IndirectValue(IndirectValue const& other):IndirectValue(*other){}
    IndirectValue(IndirectValue&& other) = default;
    IndirectValue& operator=(T&& data){ value = std::make_unique<T>(std::move(data)); return *this; }
    IndirectValue& operator=(T const& data){ value = std::make_unique<T>(data); return *this; }
    IndirectValue& operator=(IndirectValue const& other){ return *this = *other; }
    IndirectValue& operator=(IndirectValue&&) = default;
    T& operator*() &{ return *value; }
    T const& operator*() const&{ return *value; }
    T&& operator*() &&{ return std::move(*value); }
    T* get(){ return value.get(); }
    T const* get() const{ return value.get(); }
    T* operator->(){ return get(); }
    T const* operator->() const{ return get(); }
};
template<class T>
IndirectValue(T const&) -> IndirectValue<T>;
template<class T>
IndirectValue(T&&) -> IndirectValue<T>;

template<class T>
class IndirectOptional{
    std::unique_ptr<T> value;
public:
    IndirectOptional() = default;
    IndirectOptional(std::nullopt_t){}
    template<class... Args>
    IndirectOptional(std::in_place_t, Args&&... args):value(new T(std::forward<Args>(args)...)){}
    IndirectOptional(T const& value):IndirectOptional(std::in_place, value){}
    IndirectOptional(T&& value):IndirectOptional(std::in_place, value){}
    IndirectOptional(IndirectOptional const& other){
        if(other) *this = *other;
    }
    IndirectOptional(IndirectOptional&& other) = default;
    IndirectOptional& operator=(T&& data){ value = std::make_unique<T>(std::move(data)); return *this; }
    IndirectOptional& operator=(T const& data){ value = std::make_unique<T>(data); return *this; }
    IndirectOptional& operator=(IndirectOptional const& other){
        if(other) return *this = *other;
        else{
            value = nullptr;
            return *this;
        }
    }
    IndirectOptional& operator=(IndirectOptional&&) = default;
    T& operator*() &{ return *value; }
    T const& operator*() const&{ return *value; }
    T&& operator*() &&{ return std::move(*value); }
    T* get(){ return value.get(); }
    T const* get() const{ return value.get(); }
    T* operator->(){ return get(); }
    T const* operator->() const{ return get(); }
    bool hasValue() const{ return value != nullptr; }
    operator bool() const{ return value != nullptr; }
};
template<class T>
IndirectOptional(T const&) -> IndirectOptional<T>;
template<class T>
IndirectOptional(T&&) -> IndirectOptional<T>;


#endif /* IndirectValue_h */
