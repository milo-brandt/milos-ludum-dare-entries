//
//  gameInstance.h
//  StructureSmash
//
//  Created by Milo Brandt on 2/22/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef StructureSmash_gameInstance_h
#define StructureSmash_gameInstance_h

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include "ResourcePath.hpp"
#include "grid.h"
#include "player.h"
#include "terrainGen.h"

class gameInstance{
public:
    sf::Window& w;
    grid* g;
    float offX, offY,widX,widY;
    player plyr;
    int mouseType;
    bool Right,Left;
    gameInstance(sf::Window&,grid* g);
    int update();
};

#endif
