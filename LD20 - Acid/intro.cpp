/*
 *  intro.cpp
 *  Acid
 *
 *  Created by Milo Brandt on 5/1/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */

#include "intro.h"

intro::intro():x(0.f),y(140.f),chatPos(0),keyCool(0){}
void intro::keyPress(){
	if(keyCool > 0) return;
	keyCool = 15;
	if(x < 320.f) x = 320.f;
	if(chatPos > 6) x = 640.f;
	if(x == 320.f && chatPos <= 6) chatPos++;
}
bool intro::render(imageManager* mgr){
	if(keyCool > 0) keyCool--;
	if(x < 320.f || chatPos > 6){
		x += 2.f;
	}else if(chatPos == 0){
		chatPos = 1;
	}else{
		time++;
		if(time > 300){
			time = 0;
			chatPos++;
		}
	}
	if(x >= 640.f){
		return true;
	}
	glColor3f(1.f,1.f,1.f);
	glBegin(GL_LINES);
	glVertex2f(0.f, y-12.f);
	glVertex2f(640.f, y-12.f);
	glEnd();
	glPushMatrix();
	mgr->x[plyr_sprite].Bind();
	glTranslatef(x,y,0.f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.f,1.f);
	glVertex2f(-10.f,-10.f);
	glTexCoord2f(1.f,1.f);
	glVertex2f(10.f,-10.f);
	glTexCoord2f(1.f,0.f);
	glVertex2f(10.f,10.f);
	glTexCoord2f(0.f,0.f);
	glVertex2f(-10.f,10.f);
	
	glEnd();
	glPopMatrix();
	glColor3f(1.f,1.f,0.f);
	glPushMatrix();
	mgr->x[plyr_sprite].Bind();
	glTranslatef(340.f,y,0.f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.f,1.f);
	glVertex2f(-10.f,-10.f);
	glTexCoord2f(1.f,1.f);
	glVertex2f(10.f,-10.f);
	glTexCoord2f(1.f,0.f);
	glVertex2f(10.f,10.f);
	glTexCoord2f(0.f,0.f);
	glVertex2f(-10.f,10.f);
	
	glEnd();
	glPopMatrix();
	glColor3f(1.f,1.f,1.f);
	if(chatPos > 0 && chatPos <= 6){
		mgr->x[chat_sprite + chatPos].Bind();
		glBegin(GL_QUADS);
		
		glTexCoord2f(0.f,1.f);
		glVertex2f(0.f,0.f);
		glTexCoord2f(1.f,1.f);
		glVertex2f(640.f,0.f);
		glTexCoord2f(1.f,0.f);
		glVertex2f(640.f,120.f);
		glTexCoord2f(0.f,0.f);
		glVertex2f(0.f,120.f);
		
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}