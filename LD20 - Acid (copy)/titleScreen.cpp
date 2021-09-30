/*
 *  titleScreen.cpp
 *  Acid
 *
 *  Created by Milo Brandt on 4/30/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */

#include "titleScreen.h"
titleScreen::titleScreen():helpScreen(false){}
void titleScreen::render(imageManager* mgr){
	glPushMatrix();
	if(helpScreen){
		mgr->x[help_sprite].Bind();
	}else{
		mgr->x[title_sprite].Bind();
	}
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.f,1.f);
	glVertex2f(0.f,0.f);
	glTexCoord2f(1.f,1.f);
	glVertex2f(640.f,0.f);
	glTexCoord2f(1.f,0.f);
	glVertex2f(640.f,480.f);
	glTexCoord2f(0.f,0.f);
	glVertex2f(0.f,480.f);
	
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);	
}
int titleScreen::handleClick(float x,float y){
	if(!helpScreen){
		if(x > 205 && x < 450 && y > 155 && y < 245){
			return 1;
		}
		if(x > 205 && x < 450 && y > 260 && y < 350){
			helpScreen = true;
		}
	}else{
		if(x > 480 && y > 420){
			helpScreen = false;
		}
	}
	return 0;
}