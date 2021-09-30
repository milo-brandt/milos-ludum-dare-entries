/*
 *  world.cpp
 *  Acid
 *
 *  Created by Milo Brandt on 4/30/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */

#include "world.h"

void world::render(imageManager* mgr){
	glPushMatrix();
	mgr->x[goal_sprite].Bind();
	glTranslatef(goal.x,goal.y,0.f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.f,1.f);
	glVertex2f(-goal.r,-goal.r);
	glTexCoord2f(1.f,1.f);
	glVertex2f(goal.r,-goal.r);
	glTexCoord2f(1.f,0.f);
	glVertex2f(goal.r,goal.r);
	glTexCoord2f(0.f,0.f);
	glVertex2f(-goal.r,goal.r);
	
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_LINES);
	glLineWidth(1.5f);
	glColor3f(1.f,1.f,1.f);
	glBegin(GL_LINES);
	for(int i = 0;i < acidLines.size();i++){
		glVertex2f(acidLines[i]->pointA.x,acidLines[i]->pointA.y);
		glVertex2f(acidLines[i]->pointB.x,acidLines[i]->pointB.y);
	}
	glColor3f(1.f,0.f,0.f);
	for(int i = 0;i < fallLines.size();i++){
		glVertex2f(fallLines[i]->pointA.x,fallLines[i]->pointA.y);
		glVertex2f(fallLines[i]->pointB.x,fallLines[i]->pointB.y);
	}
	for(int i = 0;i < fallingLines.size();i++){
		glVertex2f(fallingLines[i]->pointA.x,fallingLines[i]->pointA.y);
		glVertex2f(fallingLines[i]->pointB.x,fallingLines[i]->pointB.y);
	}
	glColor3f(0.f,1.f,0.f);
	glLineWidth(2.f);
	for(int i = 0;i < noAcidLines.size();i++){
		glVertex2f(noAcidLines[i]->pointA.x,noAcidLines[i]->pointA.y);
		glVertex2f(noAcidLines[i]->pointB.x,noAcidLines[i]->pointB.y);
	}
	glEnd();
}