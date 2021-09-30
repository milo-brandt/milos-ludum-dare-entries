/*
 *  gameModule.cpp
 *  Acid
 *
 *  Created by Milo Brandt on 4/30/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */

#include "gameModule.h"
#include <fstream>
gameModule::gameModule(sf::Window& App,const char* file):App(App),status(0){
	lvl = new world();
	plyr = new player();
	plyr->hitBox.r = 10;
	plyr->on = 0;
	plyr->acidDur = 75.f;
	lvl->goal.r = 10;
	plyr->level = lvl;
	std::ifstream in(file);
	if(!in.is_open()){
		float err = 0.f;
	}
	in >> lvl->goal.x;
	in >> lvl->goal.y;
	in >> plyr->hitBox.x;
	in >> plyr->hitBox.y;
	int num;
	in >> num;
	for(int l = 0;l < num;l++){
		int n;
		float x1,y1,x2,y2;
		in >> n;
		in >> x1;
		in >> y1;
		in >> x2;
		in >> y2;
		if(n == 1){
			lvl->acidLines.push_back(new line(vector2(x1,y1),vector2(x2,y2)));
		}else if(n == 0){
			lvl->noAcidLines.push_back(new line(vector2(x1,y1),vector2(x2,y2)));
		}else if(n == 2){
			lvl->fallLines.push_back(new line(vector2(x1,y1),vector2(x2,y2)));
		}
	}
}
bool gameModule::handle(sf::Event evt){
	if(evt.Type == sf::Event::KeyPressed){
		if(evt.Key.Code == sf::Key::Space){
			plyr->spewAcid();
		}
		if(evt.Key.Code == sf::Key::Up){
			plyr->goUp();
		}
		if(evt.Key.Code == sf::Key::R){
			return true;
		}
	}
	return false;
}
int gameModule::update(){
 	const sf::Input& in = App.GetInput();
	if(in.IsKeyDown(sf::Key::Right)){
		plyr->goRight();
	}else if(in.IsKeyDown(sf::Key::Left)){
		plyr->goLeft();
	}else
		plyr->goNowhere();
	return plyr->update();
}
void gameModule::render(imageManager* mgr){
	glColor3f(1.f,1.f,1.f);
	plyr->render(mgr);
}