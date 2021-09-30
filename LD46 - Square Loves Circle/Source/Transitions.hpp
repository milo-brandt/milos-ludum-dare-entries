//
//  Transitions.hpp
//  Dummy
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef Transitions_hpp
#define Transitions_hpp

#include "Common/Window.h"
#include "Game/ImmediateData.h"
#include <variant>

namespace RenderEffects{
    struct LoveFade{
        Vector2<double> center;
        double startRadius;
        double endRadius;
        double lifetime;
        double time = 0;
        bool render(Window& window, ImmediateData& data, double dt);
    };
    struct NormalFade{
        double startOpacity;
        double endOpacity;
        double lifetime;
        double time = 0;
        bool render(Window& window, ImmediateData& data, double dt);
    };
    using Effect = std::variant<LoveFade, NormalFade>;
    bool render(Effect& effect, Window& window, ImmediateData& data, double dt);
};


#endif /* Transitions_hpp */
