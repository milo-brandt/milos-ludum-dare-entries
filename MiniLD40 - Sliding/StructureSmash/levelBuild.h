//
//  levelBuild.h
//  StructureSmash
//
//  Created by Milo Brandt on 2/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef StructureSmash_levelBuild_h
#define StructureSmash_levelBuild_h

#include "vector2.h"
#include <vector> //I'm a fan of vectors!

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include "ResourcePath.hpp"

class gameInstance;
class grid;
class structure{
public:
    virtual vector2 build(grid* g,vector2 s) = 0;
    static structure* makeJump(int d);
    static structure* makeSlope(int d);
    static structure* makeSlopeUp(int d);
    static structure* makeCanyon(int w,int d);
    static structure* makeWinZone(int w);
    static structure* makeBack(int h);
    virtual ~structure();
};
class levelBuild{
public:
    int anchored;
    int hover;
    int dragging;
    float dragO;
    sf::Window& w;
    std::vector<structure*> stuff;
    std::vector<vector2> trace;
    grid* g;
    std::pair<vector2, vector2> bounds;
    void generate();
    gameInstance* game();
    levelBuild(sf::Window&,std::vector<structure*>);
    bool update();
    ~levelBuild();
};

#endif
