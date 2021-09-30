//
//  titlePage.h
//  LD21
//
//  Created by Milo Brandt on 8/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef TITLE_PAGE_H
#define TITLE_PAGE_H
#include <SFML/Graphics.hpp>
#include "leveldata.h"
namespace titlePage{
    static int pageType = 0; //0 = title, 1 = help, 2 = credits.
    static sf::Image titleImage;
    static sf::Image creditsButton;
    static sf::Image playButton;
    static sf::Image helpButton;
    static sf::Image backButton;
    static sf::Image creditsPage;
    static sf::Image playButtonHard;
    static sf::Image statsPage;
    void initStuff();
    void setPageType(int p);
    void drawTitle(sf::RenderWindow& r,int mX,int mY,levelData& l);
    bool handleClick(int mX,int mY);
}
#endif