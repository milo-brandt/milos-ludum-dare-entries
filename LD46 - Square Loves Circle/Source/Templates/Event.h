//
//  Event.h
//  HandyTemplates
//
//  Created by Milo Brandt on 4/15/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef Event_h
#define Event_h

#include "TemplateUtility.h"
#include "SharedList.h"
#include <vector>
#include <iostream>
#include <cassert>

enum class EventAction : unsigned char{
    NORMAL = 0,
    DISCONNECT = 1,
    ABSORB = 2 //Event is not transmitted
};
inline EventAction operator&(EventAction a, EventAction b){ return EventAction((unsigned char)a & (unsigned char)b); };
inline EventAction operator|(EventAction a, EventAction b){ return EventAction((unsigned char)a | (unsigned char)b); };
template<class... Args>
class Event{
    using FuncType = std::function<EventAction(Args const&...)>;
    SharedList<FuncType> hooks;
public:
    class Hook{
        typename SharedList<FuncType>::Iterator it;
        Hook(typename SharedList<FuncType>::Iterator&& it):it(std::move(it)){}
        friend class Event;
    public:
        Hook() = default;
        explicit operator bool() const{ return it.connected(); }
        void disconnect(){
            it.eject();
        }
    };
    class ScopedHook : public Hook{
    public:
        ScopedHook() = default;
        ScopedHook(Hook&& hook):Hook(std::move(hook)){}
        ScopedHook(Hook const& hook):Hook(hook){}
        ScopedHook(ScopedHook&&) = default;
        ScopedHook(ScopedHook const&) = default;
        ScopedHook& operator=(ScopedHook&&) = default;
        ScopedHook& operator=(ScopedHook const&) = default;
        ~ScopedHook(){ Hook::disconnect(); }
    };
    template<class Callback, class Ret = std::invoke_result_t<Callback, Args const&...> >
    Hook hook(Callback&& callback){
        if constexpr(std::is_same_v<Ret, EventAction>){
            return {hooks.emplaceBack(std::forward<Callback>(callback))};
        }else{
            return {hooks.emplaceBack([callback = std::forward<Callback>(callback)](Args const&... args){
                callback(args...);
                return EventAction::NORMAL;
            })};
        }
    }
    template<class Callback, class Ret = std::invoke_result_t<Callback, Args const&...> >
    Hook hookOnce(Callback&& callback){
        return {hooks.emplaceBack([callback = std::forward<Callback>(callback)](Args const&... args){
            if constexpr(std::is_same_v<Ret, EventAction>){
                return callback(args...) | EventAction::DISCONNECT;
            }else{
                callback(args...);
                return EventAction::DISCONNECT;
            }
        })};
    }

    void fire(Args const&... args){
        std::vector<typename SharedList<FuncType>::Iterator> calls; //Copy out everything so that changes to list are safe.
        for(auto pos = hooks.begin(); pos != hooks.end(); ++pos){
            calls.push_back(pos);
        }
        for(auto& pos : calls){
            auto ret = (*pos)(args...);
            if((ret & EventAction::DISCONNECT) != EventAction::NORMAL) pos.eject();
            if((ret & EventAction::ABSORB) != EventAction::NORMAL) return;
        }
    }
    void operator()(Args const&... args){
        fire(args...);
    }
};
template<class T>
auto weakIndirectFunction(std::weak_ptr<T> ptr){
    return [ptr = std::move(ptr)](auto&&... args){
        if(auto locked = ptr.lock()){
            std::invoke(*locked, std::forward<decltype(args)>(args)...);
            return EventAction::NORMAL;
        }else{
            return EventAction::DISCONNECT;
        }
    };
}
template<class T, class Member>
auto weakIndirectFunction(std::weak_ptr<T> ptr, Member&& member){
    return [ptr = std::move(ptr), member = std::forward<Member>(member)](auto&&... args){
        if(auto locked = ptr.lock()){
            std::invoke(member, *locked, std::forward<decltype(args)>(args)...);
            return EventAction::NORMAL;
        }else{
            return EventAction::DISCONNECT;
        }
    };
}

template<class T>
auto weakIndirectFunction(std::shared_ptr<T> ptr){
    return weakIndirectFunction(std::weak_ptr<T>(ptr));
}
template<class T, class Member>
auto weakIndirectFunction(std::shared_ptr<T> ptr, Member&& member){
    return weakIndirectFunction(std::weak_ptr<T>(ptr), std::forward<Member>(member));
}

#endif /* Event_h */
