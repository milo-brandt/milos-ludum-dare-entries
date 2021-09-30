/*
 *  imagemgr.h
 *  Acid
 *
 *  Created by Milo Brandt on 4/30/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */
#ifndef IMAGEMGR_H
#define IMAGEMGR_H
#include <SFML/Graphics.hpp>

#define plyr_sprite 0
#define goal_sprite 1
#define complete_sprite 2
#define title_sprite 3
#define help_sprite 4
#define chat_sprite 4
#define lvl1_sprite 11
#define win_sprite 12
#define lvl2_sprite 13
class imageManager{
public:
	sf::Image x[14];
	imageManager();
};
#endif