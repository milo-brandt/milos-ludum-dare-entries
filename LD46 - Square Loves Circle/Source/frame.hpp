//
//  frame.hpp
//  Dummy
//
//  Created by Milo Brandt on 4/17/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef frame_hpp
#define frame_hpp

#include "Common/Window.h"

struct Program{
    struct Impl;
    std::unique_ptr<Impl> data;
    Program(Window&);
    ~Program();
    void frame(Window&);
};

#endif /* frame_hpp */
