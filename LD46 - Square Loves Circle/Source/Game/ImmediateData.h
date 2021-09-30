//
//  ImmediateData.h
//  WASM
//
//  Created by Milo Brandt on 4/18/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#ifndef ImmediateData_h
#define ImmediateData_h

#include "../Common/Window.h"

struct ImmediateData{
    Texture font;
    Texture white;
    Texture circle;
    Texture love;
    Texture brokenLeft;
    Texture brokenRight;
    Texture loveFader;
    SharedSample backgroundMusic;
    SharedMonoSample jump;
    SharedMonoSample bounce;
    SharedMonoSample victory;
    SharedMonoSample failure;
    SharedMonoSample click;
};


#endif /* ImmediateData_h */
