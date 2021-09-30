/*
 *  titleScreen.h
 *  Acid
 *
 *  Created by Milo Brandt on 4/30/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */
#include <SFML/Window.hpp>
#include "imagemgr.h"
class titleScreen{
public:
	bool helpScreen;
	titleScreen();
	void render(imageManager* mgr);
	int handleClick(float x,float y);
};