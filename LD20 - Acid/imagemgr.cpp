/*
 *  imagemgr.cpp
 *  Acid
 *
 *  Created by Milo Brandt on 4/30/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */

#include "imagemgr.h"

imageManager::imageManager(){
	x[plyr_sprite].LoadFromFile("player.png");
	x[goal_sprite].LoadFromFile("Goal.png");
	x[complete_sprite].LoadFromFile("complete.png");
	x[title_sprite].LoadFromFile("Title.png");
	x[help_sprite].LoadFromFile("instructions.png");
	x[chat_sprite+1].LoadFromFile("Talk1.png");
	x[chat_sprite+2].LoadFromFile("Talk2.png");
	x[chat_sprite+3].LoadFromFile("Talk3.png");
	x[chat_sprite+4].LoadFromFile("Talk4.png");
	x[chat_sprite+5].LoadFromFile("Talk5.png");
	x[chat_sprite+6].LoadFromFile("Talk6.png");
	x[lvl1_sprite].LoadFromFile("instructionsA.png");
	x[lvl2_sprite].LoadFromFile("lvl2.png");
	x[win_sprite].LoadFromFile("win.png");
}