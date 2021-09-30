//
//  TemplateUtility.h
//  FactoryGame
//
//  Created by Milo Brandt on 10/9/19.
//  Copyright © 2019 Milo Brandt. All rights reserved.
//

#ifndef TemplateUtility_h
#define TemplateUtility_h

#include <tuple>
#include <array>
#include <memory>
#include <vector>
#include <functional>

//Helpers
template<class T, class Callback>
void eraseFromVectorIf(std::vector<T>& vec, Callback&& callback){
    auto start = std::remove_if(vec.begin(), vec.end(), std::forward<Callback>(callback));
    vec.erase(start, vec.end());
}

//From cppreference
namespace Detail {
    template <class>
    constexpr bool is_reference_wrapper_v = false;
    template <class U>
    constexpr bool is_reference_wrapper_v<std::reference_wrapper<U>> = true;

    template <class T, class Type, class T1, class... Args>
    constexpr decltype(auto) INVOKE(Type T::* f, T1&& t1, Args&&... args)
    {
        if constexpr (std::is_member_function_pointer_v<decltype(f)>) {
            if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
                return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
            else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
                return (t1.get().*f)(std::forward<Args>(args)...);
            else
                return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
        } else {
            static_assert(std::is_member_object_pointer_v<decltype(f)>);
            static_assert(sizeof...(args) == 0);
            if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
                return std::forward<T1>(t1).*f;
            else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
                return t1.get().*f;
            else
                return (*std::forward<T1>(t1)).*f;
        }
    }

    template <class F, class... Args>
    constexpr decltype(auto) INVOKE(F&& f, Args&&... args)
    {
        return std::forward<F>(f)(std::forward<Args>(args)...);
    }
}

template< class F, class... Args>
constexpr std::invoke_result_t<F, Args...> constexprInvoke(F&& f, Args&&... args)
noexcept(std::is_nothrow_invocable_v<F, Args...>)
{
    return Detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
}



/* Tuple iteration helpers */

template<class Fn, class Tuple>
constexpr void forEachInTuple(Fn&& f, Tuple&& tuple){
    std::apply([&f](auto&&... args){
        (std::invoke(f, args) , ...);
    }, std::forward<Tuple>(tuple));
}
template<class Fn, class Tuple>
constexpr auto transformTuple(Fn&& f, Tuple&& tuple){
    return std::apply([&f](auto&&... args){
        return std::make_tuple(std::invoke(f, args)...);
    }, std::forward<Tuple>(tuple));
}

/* Array iteration helpers */

template<class Fn, class T, std::size_t N>
constexpr void forEachInArray(Fn&& f, std::array<T, N> const& arr){
    std::apply([&f](auto&&... args){
        (constexprInvoke(f, args) , ...);
    }, arr);
}
template<class Fn, class T, std::size_t N, class Ret = std::invoke_result_t<Fn, T> >
constexpr auto transformArray(Fn&& f, std::array<T, N> const& arr){
    return std::apply([&f](auto&&... args){
        return std::array<Ret, N>(constexprInvoke(f, args)...);
    }, arr);
}
template<std::size_t N>
constexpr std::size_t countInArray(std::array<bool, N> const& arr){
    std::size_t ret = 0;
    forEachInArray([&ret](bool b){ ret += b; }, arr);
    return ret;
}
template<std::size_t N>
constexpr std::size_t uniqueTrueInArray(std::array<bool, N> const& arr){
    return countInArray(arr) == 1;
}
template<std::size_t N>
constexpr std::size_t indexOfTrueInArray(std::array<bool, N> const& arr){
    std::size_t ret = -1;
    std::size_t pos = 0;
    forEachInArray([&ret, &pos](bool b){
        if(b) ret = pos;
        ++pos;
    }, arr);
    return ret;
}
template<std::size_t N>
constexpr bool anyInArray(std::array<bool, N> const& arr){
    bool ret = false;
    forEachInArray([&ret](bool b){ ret = ret || b; }, arr);
    return ret;
}
template<std::size_t N>
constexpr bool allInArray(std::array<bool, N> const& arr){
    bool ret = true;
    forEachInArray([&ret](bool b){ ret = ret && b; }, arr);
    return ret;
}


/* Lambda helpers */

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; }; //Wonderful trick stolen from Bjarne.
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct ignore_undefined_t{
    template<class... Args>
    void operator()(Args&&...) const{}; //Lowest priority for matching
};
constexpr ignore_undefined_t ignore_undefined;

/* Template wrappers */

template<auto V>
struct ValueFlag{
    static constexpr auto value = V;
};
template<class T>
struct ClassFlag{
    using Type = T;
};
template<class... Args>
struct ClassList{
    template<template<class...> class Head>
    using Apply = Head<Args...>;
    template<template<class...> class Map>
    using Map = ClassList<Map<Args>...>;
    template<template<class...> class Transform>
    using Transform = ClassList<typename Transform<Args>::Type...>;
    template<class... Added>
    using Append = ClassList<Args..., Added...>;
    template<class... Added>
    using Prepend = ClassList<Added..., Args...>;
    template<class T>
    using Join = typename T::template Prepend<Args...>;
};
template<auto Fn>
struct StaticFunction{ //A default-constructible wrapper for a specific function.
    static_assert(std::is_function_v<decltype(Fn)>, "Only functions can be used as static functions.");
};
template<class Ret, class... Args, Ret(*fn)(Args...)>
struct StaticFunction<fn>{
    static constexpr auto function = fn;
    Ret operator()(Args... args){
        return fn(args...);
    };
};

/* Constexpr-friendly range manipulations */

namespace Detail{
    template<class T, std::size_t N, class Callback, std::size_t... indices>
    constexpr std::array<T,N> __generateStaticArray(Callback&& callback, std::index_sequence<indices...>){
        static_assert((std::is_invocable_r_v<T, Callback, ValueFlag<indices> > && ...), "The callback must be callable with ValueFlag<n> for 0 < n < N and return the proper type.");
        return {callback(ValueFlag<indices>{})...};
    }
    template<class Callback, std::size_t... indices>
    constexpr auto __generateStaticTuple(Callback&& callback){
        static_assert((std::is_object_v<std::invoke_result_t<Callback, ValueFlag<indices> > > && ...), "The callback must be callable with ValueFlag<n> for 0 < n < N and return an object.");
        return std::make_tuple(callback(ValueFlag<indices>{})...);
    }
    template<template<auto...> class, class>
    struct __generateClassList;
    template<template<auto...> class Transform, std::size_t... indices>
    struct __generateClassList<Transform, std::index_sequence<indices...> >{
        using Type = ClassList<Transform<indices>...>;
    };
}
template<class T, std::size_t N, class Callback>
constexpr std::array<T,N> generateStaticArray(Callback&& callback){ //Calls callback with ValueFlag<0>{}, ...
    return Detail::__generateStaticArray<T, N>(callback, std::make_index_sequence<N>{});
}
template<std::size_t N, class Callback>
constexpr auto generateStaticTuple(Callback&& callback){ //Calls callback with ValueFlag<0>{}, ...
    return Detail::__generateStaticTuple(callback, std::make_index_sequence<N>{});
}
template<std::size_t N, template<auto...> class Transform>
using GenerateClassList = typename Detail::__generateClassList<Transform, std::make_index_sequence<N> >::Type;

/* Parameter pack helpers */

namespace Detail{
    template<std::size_t index, class... N>
    struct __IndexExtractor;
    template<class First, class... Rest>
    struct __IndexExtractor<0, First, Rest...>{
        using Type = First;
    };
    template<std::size_t index, class First, class... Rest>
    struct __IndexExtractor<index, First, Rest...> : __IndexExtractor<index - 1, Rest...>{};
    template<std::size_t index, class... N>
    struct __CheckedExtractor{
        static_assert(index < sizeof...(N), "Index must be less than size of parameter pack.");
        using Type = typename __IndexExtractor<index, N...>::Type;
    };




};
template<std::size_t index, class... N>
using NthParameter = typename Detail::__CheckedExtractor<index, N...>::Type;
template<class T, class... N>
static constexpr bool PackIncludes = anyInArray(std::array<bool, sizeof...(N)>{std::is_same_v<T, N>...});
template<class T, class... N>
static constexpr bool PackIncludesUnique = uniqueTrueInArray(std::array<bool, sizeof...(N)>{std::is_same_v<T, N>...});
template<class T, class... N>
static constexpr std::size_t PackIncludesUniqueIndex = indexOfTrueInArray(std::array<bool, sizeof...(N)>{std::is_same_v<T, N>...});

/* Calling helpers */

namespace Detail{
    template<bool Call,class Fn,class... Args>
    struct __maybeCall{};
    template<class Fn,class... Args>
    struct __maybeCall<true,Fn,Args...>{
        static constexpr  void call(Fn&& f,Args&&... args){
            std::invoke(std::forward<Fn>(f),std::forward<Args>(args)...);
        }
    };
    template<class Fn,class... Args>
    struct __maybeCall<false,Fn,Args...>{
        static constexpr void call(Fn&& f,Args&&... args){}
    };
}
template<class Fn,class... Args>
constexpr void maybeCall(Fn&& f,Args&&... args){
    Detail::__maybeCall<std::is_invocable_v<Fn, Args...>, Fn&&, Args&&...>::call(std::forward<Fn>(f), std::forward<Args>(args)...);
}

/* Pointer helpers */

template<class Ptr>
auto indirectFunction(Ptr&& ptr){
    return [ptr = std::forward<Ptr>(ptr)](auto&&... args){
        return std::invoke(*ptr, std::forward<decltype(args)>(args)...);
    };
}
template<class Ptr, class Member>
auto indirectFunction(Ptr&& ptr, Member&& member){
    return [ptr = std::forward<Ptr>(ptr), member = std::forward<Member>(member)](auto&&... args){
        return std::invoke(member, *ptr, std::forward<decltype(args)>(args)...);
    };
}
template<class T>
std::shared_ptr<T> copyToShared(T value){
    return std::make_shared<T>(std::move(value));
}
template<class T>
auto copyToIndirectFunction(T value){
    return indirectFunction(copyToShared(std::move(value)));
}
template<class T, class Member>
auto copyToIndirectFunction(T value, Member&& member){
    return indirectFunction(copyToShared(value), std::forward<Member>(member));
}

#endif /* TemplateUtility_h */
