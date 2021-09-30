//
//  imageManager.cpp
//  DwarvenIsolation
//
//  Created by Milo Brandt on 12/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "imageManager.h"

imageManager::imageManager():imgs(new sf::Image[20]),sounds(new sf::Music[5]){
    rocks = 0;
    imgs[rocks].LoadFromFile("rock.png");
    imgs[rocks + 1].LoadFromFile("rock2.png");
    imgs[rocks + 2].LoadFromFile("rock3.png");
    imgs[rocks + 3].LoadFromFile("copper.png");
    imgs[rocks + 4].LoadFromFile("ruby.png");
    imgs[rocks + 5].LoadFromFile("gold.png");
    imgs[rocks + 6].LoadFromFile("diamond.png");
    unbreakable = rocks + 7;
    imgs[unbreakable].LoadFromFile("unbreakable.png");
    dwarf = unbreakable + 1;
    imgs[dwarf].LoadFromFile("dwarf.png");
    dead = dwarf + 1;
    imgs[dead].LoadFromFile("deaddwarf.png");
    sticky = dead + 1;
    imgs[sticky].LoadFromFile("sticky.png");
    deadly = sticky + 1;
    imgs[deadly].LoadFromFile("spikes.png");
    shinyCoin = deadly + 1;
    imgs[shinyCoin].LoadFromFile("happyCoin.png");
    doneButton = shinyCoin + 1;
    imgs[doneButton].LoadFromFile("donebutton.png");
    emptyButton = doneButton + 1;
    imgs[emptyButton].LoadFromFile("emptytile.png");
    stickyButton = emptyButton + 1;
    imgs[stickyButton].LoadFromFile("stickytrapbutton.png");
    deadlyButton = stickyButton + 1;
    imgs[deadlyButton].LoadFromFile("killtrapbutton.png");
    shinyButton = deadlyButton + 1;
    imgs[shinyButton].LoadFromFile("shiny.png");
    menuButton = shinyButton + 1;
    imgs[menuButton].LoadFromFile("menu.png");
    menu = menuButton + 1;
    imgs[menu].LoadFromFile("mainmenu.png");
    sounds[0].OpenFromFile("splat.aif");
    sounds[1].OpenFromFile("ding.aif");
    sounds[2].OpenFromFile("die.aif");
    sounds[3].OpenFromFile("boom 1.aif");
    sounds[4].OpenFromFile("scream.aif");
}
sf::Image& imageManager::imgFromValue(unsigned int v){
    if(v < 5) return imgs[rocks];
    if(v < 15) return imgs[rocks + 1];
    if(v < 25) return imgs[rocks + 2];
    if(v < 40) return imgs[rocks + 3];
    if(v < 65) return imgs[rocks + 4];
    if(v < 90) return imgs[rocks + 5];
    return imgs[rocks + 6];
}