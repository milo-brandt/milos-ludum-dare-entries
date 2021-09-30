/*
 *  gameModule.h
 *  Acid
 *
 *  Created by Milo Brandt on 4/30/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */
#include "player.h"
#include <SFML/Window.hpp>

class gameModule{
public:
	world* lvl;
	player* plyr;
	int status;
	const sf::Window& App;
	gameModule(sf::Window& App,const char* file);
	int update();
	bool handle(sf::Event evt);
	void render(imageManager* mgr);
};