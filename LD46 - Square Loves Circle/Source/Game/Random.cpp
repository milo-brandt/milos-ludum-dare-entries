//
//  Random.cpp
//  Dummy
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#include "Random.hpp"

std::default_random_engine& getRandomGenerator(){
    static std::default_random_engine generator;
    return generator;
}
double getRandomUniform(double low, double high){
    std::uniform_real_distribution<double> distribution(low, high);
    return distribution(getRandomGenerator());
}
double getRandomNormal(){
    static std::normal_distribution<double> distribution;
    return distribution(getRandomGenerator());;
}
Vector2<double> getRandomNormal2(){
    return {getRandomNormal(), getRandomNormal()};
}
Vector2<double> getRandomInRectangle(Rectangle<double> const& region){
    return {getRandomUniform(region.lesser[0], region.greater[0]), getRandomUniform(region.lesser[1], region.greater[1])};
}
