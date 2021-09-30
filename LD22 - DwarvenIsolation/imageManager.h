//
//  imageManager.h
//  DwarvenIsolation
//
//  Created by Milo Brandt on 12/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef imageManager_H_DEF
#define imageManager_H_DEF
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class imageManager{
public:
    unsigned int rocks;
    unsigned int unbreakable;
    unsigned int dwarf;
    unsigned int dead;
    unsigned int sticky;
    unsigned int deadly;
    unsigned int shinyCoin;
    unsigned int doneButton;
    unsigned int emptyButton;
    unsigned int stickyButton;
    unsigned int deadlyButton;
    unsigned int shinyButton;
    unsigned int menuButton;
    unsigned int menu;
    sf::Image* imgs;
    sf::Music* sounds;
    sf::Image& imgFromValue(unsigned int v);
    imageManager();
};

#endif