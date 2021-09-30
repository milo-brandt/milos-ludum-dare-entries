//
//  KeyboardHandler.cpp
//  WASM
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#include "KeyboardHandler.h"

#include <SDL.h>

std::optional<Key> processKeycode(int keycode){
    if(keycode == SDLK_LEFT || keycode == SDLK_a) return Key::LEFT;
    if(keycode == SDLK_RIGHT || keycode == SDLK_d) return Key::RIGHT;
    if(keycode == SDLK_DOWN || keycode == SDLK_s) return Key::DOWN;
    if(keycode == SDLK_SPACE || keycode == SDLK_UP || keycode == SDLK_w) return Key::SPACE;
    //if(keycode == SDLK_x) return Key::SKIP_LEVEL; NO CHEATING
    if(keycode == SDLK_r) return Key::RESTART;

    return std::nullopt;
}
