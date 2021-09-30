//
//  Transitions.cpp
//  Dummy
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#include "Transitions.hpp"

namespace RenderEffects{
    bool LoveFade::render(Window &window, ImmediateData &data, double dt){
        time += dt;
        if(time > lifetime) time = lifetime;
        double progress = time / lifetime;
        float rad = float(startRadius * (1 - progress) + endRadius * progress);
        Vector2<float> cent = center;
        RenderingSegment frame;
        frame.addRectangle({{-2, -1}, {cent[0] - rad, 1}}, standardTextureRect, {0.f, 0.f, 0.f, 1.f});
        frame.addRectangle({{cent[0] + rad, -1}, {2, 1}}, standardTextureRect, {0.f, 0.f, 0.f, 1.f});
        frame.addRectangle({{-2, -1}, {2, cent[1] - rad}}, standardTextureRect, {0.f, 0.f, 0.f, 1.f});
        frame.addRectangle({{-2, cent[1] + rad}, {2, 1}}, standardTextureRect, {0.f, 0.f, 0.f, 1.f});
        RenderingSegment heart;
        heart.addRectangle(Rectangle<float>::squareFromCenter(center, rad), oppositeTextureRect);
        window.setTexture(data.white);
        window.draw(frame);
        window.setTexture(data.loveFader);
        window.draw(heart);
        return time + dt*0.01 >= lifetime;
    }
    bool NormalFade::render(Window &window, ImmediateData &data, double dt){
        time += dt;
        if(time > lifetime) time = lifetime;
        double progress = time / lifetime;
        RenderingSegment panel;
        float opacity = float(startOpacity * (1 - progress) + endOpacity * progress);
        panel.addRectangle({{-2, -1}, {2, 1}}, standardTextureRect, {0.f, 0.f, 0.f, opacity});
        window.setTexture(data.white);
        window.draw(panel);
        return time + dt*0.01 >= lifetime;
    }

    bool render(Effect& effect, Window& window, ImmediateData& data, double dt){
        return std::visit([&](auto& effect) -> bool{
            return effect.render(window, data, dt);
        }, effect);
    }
}
