/*
 *  intro.h
 *  Acid
 *
 *  Created by Milo Brandt on 5/1/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */
#include "imagemgr.h"
class intro{
public:
	float x,y;
	int chatPos;
	int time;
	int keyCool;
	intro();
	void keyPress();
	bool render(imageManager* mgr);
};