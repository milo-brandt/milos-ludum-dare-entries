/*
 *  player.h
 *  Acid
 *
 *  Created by Milo Brandt on 4/30/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */
#include "imagemgr.h"
#include "line.h"
#include "world.h"
class player{
public:
	world* level;
	circle hitBox;
	float ang;
	float xSpeed;
	float ySpeed;
	line* on;
	bool jumped;
	std::vector<float> acidDir;
	float acidDur;
	vector2 acidCent;
	bool spewAcid();
	void goNowhere();
	void goLeft();
	void goRight();
	void goUp();
	int update();
	void render(imageManager* mgr);
	bool checkHit(line* b);
	void checkHit(int x = 50);
};