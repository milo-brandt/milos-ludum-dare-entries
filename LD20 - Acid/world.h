/*
 *  world.h
 *  Acid
 *
 *  Created by Milo Brandt on 4/30/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */
#include <vector>
#include "line.h"
#include "imagemgr.h"
class world{
public:
	std::vector<line*> acidLines;
	std::vector<line*> noAcidLines;
	std::vector<line*> fallLines;
	std::vector<line*> fallingLines;
	circle goal;
	void render(imageManager* mgr);
};