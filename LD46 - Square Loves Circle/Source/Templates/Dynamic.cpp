//
//  Dynamic.cpp
//  FactoryGame
//
//  Created by Milo Brandt on 3/11/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#include "Dynamic.h"
#include "Cubic.h"

SharedDynamic<double> makeSmoothBoolean(SharedDynamic<bool> value, double delay, SharedDynamic<double> clock){
    struct ComputationFunc{
        double delay;
        SharedDynamic<double> clock;
        SharedDynamic<bool> value;
        Dynamic<double>* me;
        Cubic<double> cubic = {double(value->get()), 0, 0, 0};
        double finishingTime = clock->get();
        ComputationFunc(double delay, SharedDynamic<double>&& clock, SharedDynamic<bool>&& value, Dynamic<double>* me):delay(delay),clock(std::move(clock)),value(std::move(value)),me(me){}
        Event<>::ScopedHook clockHook;
        void valueChanged(bool newValue){
            double remaining = finishingTime - clock->get();
            double evaluation = remaining < 0 ? cubic.coefficients[0] : cubic.evaluate(remaining);
            double rate = remaining < 0 ? 0 : cubic.evaluateDerivative(remaining);
            cubic = Cubic<double>::fromControls({0, (double)newValue, 0}, {delay, evaluation, rate});
            finishingTime = clock->get() + delay;
            if(!clockHook)
                clockHook = clock->invalidated.hook(invalidatorFor(me));
        }
        Event<bool>::ScopedHook valueHook = value->updated.hook(std::bind(&ComputationFunc::valueChanged, this, std::placeholders::_1));
        double operator()(){
            double remaining = finishingTime - clock->get();
            if(remaining < 0){
                clockHook.disconnect();
                return cubic.coefficients[0];
            }else{
                return cubic.evaluate(remaining);
            }
        }
    };
    auto ret = makeDynamic<double>();
    ret->setCallback(indirectFunction(std::make_shared<ComputationFunc>(delay, std::move(clock), std::move(value), ret.get())));
    return ret;
}
SharedDynamic<double> makeGatedOscillator(SharedDynamic<bool> gate, double period, SharedDynamic<double> clock){
    struct ComputationFunc{
        double period;
        SharedDynamic<double> clock;
        SharedDynamic<bool> gate;
        Dynamic<double>* me;
        double finishingTime = clock->get();
        ComputationFunc(double period, SharedDynamic<double>&& clock, SharedDynamic<bool>&& _gate, Dynamic<double>* me):period(period),clock(std::move(clock)),gate(std::move(_gate)),me(me){
            if(gate->get()) gateChanged(true);
        }
        Event<>::ScopedHook clockHook;
        void gateChanged(bool newValue){
            if(newValue && finishingTime <= clock->get()){
                finishingTime = clock->get() + period;
                clockHook = clock->invalidated.hook(invalidatorFor(me));
            }
        }
        Event<bool>::ScopedHook valueHook = gate->updated.hook(std::bind(&ComputationFunc::gateChanged, this, std::placeholders::_1));
        double operator()(){
            while(true){
                double remaining = finishingTime - clock->get();
                if(remaining > 0) return 1 - remaining / period;
                if(gate->get()) finishingTime += period; //Loop
                else{
                    clockHook.disconnect();
                    return 0;
                }
            }
        }
    };
    auto ret = makeDynamic<double>();
    ret->setCallback(indirectFunction(std::make_shared<ComputationFunc>(period, std::move(clock), std::move(gate), ret.get())));
    return ret;
}
TriggeredEnvelope makeTriggeredEnvelope(double decayTime, SharedDynamic<double> clock){
    struct ComputationFunc{
        double decayTime;
        SharedDynamic<double> clock;
        Dynamic<double>* me;
        double finishingTime = clock->get();
        void trigger(){
            finishingTime = clock->get() + decayTime;
            if(!clockHook)
                clockHook = clock->invalidated.hook(invalidatorFor(me));
        }
        ComputationFunc(double decayTime, SharedDynamic<double>&& clock, Dynamic<double>* me):decayTime(decayTime),clock(std::move(clock)),me(me){}
        Event<>::ScopedHook clockHook;
        double operator()(){
            double remaining = finishingTime - clock->get();
            if(remaining < 0){
                clockHook.disconnect();
                return 0;
            }else{
                return remaining / decayTime;
            }
        }
    };
    TriggeredEnvelope ret{makeDynamic<double>(), {}};
    auto f = std::make_shared<ComputationFunc>(decayTime, std::move(clock), ret.value.get());
    ret.value->setCallback(indirectFunction(f));
    ret.trigger = weakIndirectFunction(f, &ComputationFunc::trigger);
    return ret;
}
DecayingSum makeDecayingSum(SharedDynamic<double> clock){
    struct ComputationFunc{
        SharedDynamic<double> clock;
        Dynamic<double>* me;
        Event<>::ScopedHook triggerHook;
        double finishingTime = clock->get();
        void add(double time){
            finishingTime = std::max(finishingTime, clock->get()) + time;
            if(!clockHook)
                clockHook = clock->invalidated.hook(invalidatorFor(me));
        }
        ComputationFunc(SharedDynamic<double>&& clock, Dynamic<double>* me):clock(std::move(clock)),me(me){}
        Event<>::ScopedHook clockHook;
        double operator()(){
            double remaining = finishingTime - clock->get();
            if(remaining < 0){
                clockHook.disconnect();
                return 0;
            }else{
                return remaining;
            }
        }
    };
    DecayingSum ret{makeDynamic<double>(), {}};
    auto f = std::make_shared<ComputationFunc>(std::move(clock), ret.value.get());
    ret.value->setCallback(indirectFunction(f));
    ret.add = weakIndirectFunction(f, &ComputationFunc::add);
    return ret;
}
