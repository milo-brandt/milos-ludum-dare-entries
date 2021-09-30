//
//  KeyboardHandler.hpp
//  WASM
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef KeyboardHandler_hpp
#define KeyboardHandler_hpp

#include <optional>

enum struct Key{
    LEFT, RIGHT, SPACE, DOWN, SKIP_LEVEL, RESTART
};
std::optional<Key> processKeycode(int);

#endif /* KeyboardHandler_hpp */
