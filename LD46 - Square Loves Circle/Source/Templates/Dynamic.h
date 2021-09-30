//
//  Dynamic.h
//  FactoryGame
//
//  Created by Milo Brandt on 3/11/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef Dynamic_h
#define Dynamic_h

#include "Event.h"

template<class T>
class Dynamic{
    std::function<T()> computeCallback;
    std::optional<T> value;
public:
    Event<> invalidated;
    Event<T> updated;
    Dynamic() = default;
    explicit Dynamic(T const& value):value(value){}
    explicit Dynamic(T&& value):value(std::move(value)){}
    Dynamic(Dynamic const&) = delete; //Dynamics are resources.
    Dynamic(Dynamic&& other) = delete;
    Dynamic& operator=(Dynamic const&) = delete;
    Dynamic& operator=(Dynamic&& other) = delete;
    void invalidate(){
        invalidated.fire();
        value.reset();
    }
    void setCallback(std::function<T()> nextCallback){
        computeCallback = std::move(nextCallback);
    }
    T const& get(){
        if(value) return *value;
        else{
            assert(computeCallback);
            value = computeCallback();
            updated.fire(*value);
            return *value;
        }
    }
    T const& operator*(){ return get(); }
    T const* operator->(){ return &get(); }
    template<class... Args>
    Dynamic& emplaceValue(Args&&... args){
        assert(!computeCallback);
        if(value) invalidated.fire();
        value.emplace(std::forward<Args>(args)...);
        updated.fire(*value);
        return *this;
    }
    Dynamic& set(T const& nextValue){
        return emplaceValue(nextValue);
    }
    Dynamic& set(T&& nextValue){
        return emplaceValue(std::move(nextValue));
    }
    Dynamic& setIfNotEqual(T const& nextValue){
        if(value && value == nextValue) return *this;
        return emplaceValue(nextValue);
    }
    Dynamic& setIfNotEqual(T&& nextValue){
        if(value && value == nextValue) return *this;
        return emplaceValue(std::move(nextValue));
    }

    Dynamic& operator=(T const& nextValue){
        return emplaceValue(nextValue);
    }
    Dynamic& operator=(T&& nextValue){
        return emplaceValue(std::move(nextValue));
    }
};
template<class T>
struct SharedDynamic : std::shared_ptr<Dynamic<T> >{
    using std::shared_ptr<Dynamic<T> >::shared_ptr;
    SharedDynamic():std::shared_ptr<Dynamic<T> >(new Dynamic<T>{}){}
    SharedDynamic(std::shared_ptr<Dynamic<T> > const& ptr):std::shared_ptr<Dynamic<T> >(ptr){}
    SharedDynamic(std::shared_ptr<Dynamic<T> >&& ptr):std::shared_ptr<Dynamic<T> >(ptr){}

};
template<class T>
struct WeakDynamic : std::weak_ptr<Dynamic<T> >{
    using std::weak_ptr<Dynamic<T> >::weak_ptr;
    WeakDynamic(std::weak_ptr<Dynamic<T> > const& ptr):std::weak_ptr<Dynamic<T> >(ptr){}
    WeakDynamic(std::weak_ptr<Dynamic<T> >&& ptr):std::weak_ptr<Dynamic<T> >(ptr){}
    SharedDynamic<T> lock() const{
        return {std::weak_ptr<Dynamic<T> >::lock()};
    }
};
template<class T>
auto invalidatorFor(Dynamic<T>* target){ //Pointer should be valid longer than invalidator lives.
    return [target](){ target->invalidate(); };
}
template<class T>
auto invalidatorFor(SharedDynamic<T> target){
    return [target = std::move(target)](){ target->invalidate(); };
}
template<class T>
auto invalidatorFor(WeakDynamic<T> target){
    return [target = std::move(target)](){
        if(auto lock = target.lock()){
            lock->invalidate();
            return EventAction::NORMAL;
        }else{
            return EventAction::DISCONNECT;
        }
    };
}

template<class T>
struct Toggleable{
    class Handle{
        SharedDynamic<T> _dynamic;
        std::function<void()> _invalidator;
        Event<>::ScopedHook _hook;
    public:
        template<class U>
        Handle(SharedDynamic<T> dynamic, Dynamic<U>* parent):_dynamic(std::move(dynamic)),_invalidator(invalidatorFor(parent)),_hook(_dynamic->invalidated.hook(_invalidator)){}
        void hook(){
            if(!_hook)
                _hook = _dynamic->invalidated.hook(_invalidator);
        }
        void disconnect(){
            _hook.disconnect();
        }
        T const& get() const{ return **_dynamic; }
        T const& operator*() const{ return **_dynamic; }
    };
    SharedDynamic<T> dynamic;
    Toggleable(SharedDynamic<T> dynamic):dynamic(std::move(dynamic)){}
};
template<class T>
Toggleable(SharedDynamic<T> dynamic) -> Toggleable<T>;

namespace Detail{
    template<class T>
    struct ArgDecay{
        using PassedType = T const&;
        using StoredType = T;
        static PassedType extract(StoredType& value){
            return value;
        }
        template<class Func, class U>
        static StoredType store(T value, Dynamic<U>*){ return value; }
    };
    template<class T>
    struct ArgDecay<T&> : ArgDecay<T>{};
    template<class T>
    struct ArgDecay<T&&> : ArgDecay<T>{};
    template<class T>
    struct ArgDecay<T const> : ArgDecay<T>{};
    template<class T>
    struct ArgDecay<SharedDynamic<T> >{
        using PassedType = T const&;
        struct StoredType{
            SharedDynamic<T> value;
            Event<>::ScopedHook hook;
        };
        static PassedType extract(StoredType& value){
            return **value.value;
        }
        template<class U>
        static StoredType store(SharedDynamic<T> value, Dynamic<U>* parent){
            return { value, value->invalidated.hook(invalidatorFor(parent)) };
        }
    };
    template<class T>
    struct ArgDecay<Toggleable<T> >{
        using StoredType = typename Toggleable<T>::Handle;
        using PassedType = StoredType&;
        static PassedType extract(StoredType& value){
            return value;
        }
        template<class U>
        static StoredType store(Toggleable<T> value, Dynamic<U>* parent){
            return { value.dynamic, parent };
        }
    };
};
template<class ReturnType, class Func, class... Args, class = std::enable_if_t<std::is_invocable_r_v<ReturnType, Func, typename Detail::ArgDecay<Args>::PassedType...> > >
void setupDynamic(Dynamic<ReturnType>& ret, Func&& f, Args&&... args){
    auto argTuple = std::make_tuple(Detail::ArgDecay<Args>::store(std::forward<Args>(args), &ret)...);
    auto argStore = std::make_shared<decltype(argTuple)>(std::move(argTuple));
    ret.setCallback([f, argStore]() mutable{
        return std::apply([&f](auto&... args){ return std::invoke(f, Detail::ArgDecay<Args>::extract(args)...); }, *argStore);
    });
}
template<class Func, class... Args, class ReturnType = std::invoke_result_t<Func, typename Detail::ArgDecay<Args>::PassedType...>, class = std::enable_if_t<std::is_object_v<ReturnType> > >
SharedDynamic<ReturnType> makeDynamic(Func&& f, Args&&... args){
    auto ret = std::make_shared<Dynamic<ReturnType> >();
    setupDynamic(*ret, std::forward<Func>(f), std::forward<Args>(args)...);
    return ret;
}
template<class T>
SharedDynamic<T> makeDynamic(T value){
    return std::make_shared<Dynamic<T> >(std::move(value));
}
template<class T>
SharedDynamic<T> makeDynamic(){
    return std::make_shared<Dynamic<T> >();
}

template<class A, class B>
auto operator+(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a + b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator+(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a + b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator+(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a + b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator*(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a * b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator*(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a * b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator*(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a * b; }, std::move(a), std::move(b));
}
template<class A>
auto operator-(SharedDynamic<A> a){
    return makeDynamic([](A const& a){ return -a; }, std::move(a));
}
template<class A, class B>
auto operator-(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a - b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator-(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a - b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator-(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a - b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator&&(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a && b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator&&(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a && b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator&&(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a && b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator||(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a || b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator||(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a || b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator||(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a || b; }, std::move(a), std::move(b));
}
template<class A>
auto operator!(SharedDynamic<A> a){
    return makeDynamic([](A const& a){ return !a; }, std::move(a));
}
template<class A, class B>
auto operator<(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a < b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator<(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a < b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator<(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a < b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator<=(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a <= b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator<=(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a <= b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator<=(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a <= b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator>(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a > b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator>(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a > b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator>(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a > b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator>=(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a >= b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator>=(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a >= b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator>=(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a >= b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator==(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a == b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator==(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a == b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator==(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a == b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator!=(A a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a != b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator!=(SharedDynamic<A> a, B b){
    return makeDynamic([](A const& a, B const& b){ return a != b; }, std::move(a), std::move(b));
}
template<class A, class B>
auto operator!=(SharedDynamic<A> a, SharedDynamic<B> b){
    return makeDynamic([](A const& a, B const& b){ return a != b; }, std::move(a), std::move(b));
}

SharedDynamic<double> makeSmoothBoolean(SharedDynamic<bool> value, double delay, SharedDynamic<double> clock);
SharedDynamic<double> makeGatedOscillator(SharedDynamic<bool> gate, double period, SharedDynamic<double> clock);
struct TriggeredEnvelope{
    SharedDynamic<double> value;
    std::function<EventAction()> trigger;
};
TriggeredEnvelope makeTriggeredEnvelope(double decayTime, SharedDynamic<double> clock);
struct DecayingSum{
    SharedDynamic<double> value;
    std::function<EventAction(double)> add;
};
DecayingSum makeDecayingSum(SharedDynamic<double> clock);

#endif /* Dynamic_h */
