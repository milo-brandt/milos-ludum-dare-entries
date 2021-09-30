//
//  Random.hpp
//  Dummy
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef Random_hpp
#define Random_hpp

#include "../Templates/Vector.h"
#include "../Templates/Shapes.h"
#include <random>

std::default_random_engine& getRandomGenerator();
double getRandomUniform(double low, double high);
double getRandomNormal();
Vector2<double> getRandomNormal2();
Vector2<double> getRandomInRectangle(Rectangle<double> const&);

#endif /* Random_hpp */
