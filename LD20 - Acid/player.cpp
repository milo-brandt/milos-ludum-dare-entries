#include "player.h"
#include <SFML/Window.hpp>
#define PLAYER_ACC_X 1.f
#define PLAYER_AIR_PENALTY 1.f
#define PLAYER_GRAVITY 0.5f
#define PLAYER_SLOWDOWN 0.91f
#define RED_FALL 25.f
int player::update(){
	acidDur += 1;
	circle hit;
	hit.x = acidCent.x;
	hit.y = acidCent.y;
	hit.r = 50.f;
	for(int i = 0;i < level->fallingLines.size();i++){
		level->fallingLines[i]->pointA.y -= 3;
		level->fallingLines[i]->pointB.y -= 3;
		if(level->fallingLines[i] == on){
			hitBox.y-=3;
		}
		if(level->fallingLines[i]->pointA.y < 0 && level->fallingLines[i]->pointB.y < 0){
			if(level->fallingLines[i] == on){
				on = 0;
			}
			level->fallingLines.erase(level->fallingLines.begin() + i);
			i--;
		}
	}
	if(acidDur == 25){
		for(int i = 0;i < level->acidLines.size();i++){
			line* r = level->acidLines[i]->splitBy(hit);
			if(r != 0){
				if(level->acidLines[i] == on) on = 0;
				if(r[0].pointA.x == 1000){
					level->acidLines.erase(level->acidLines.begin()+i);
					i--;
				}else if(r[1].pointA.x == 1000){
					level->acidLines.erase(level->acidLines.begin()+i);
					level->acidLines.insert(level->acidLines.begin(),&r[0]);
				}else{
					level->acidLines.erase(level->acidLines.begin()+i);
					level->acidLines.insert(level->acidLines.begin(),&r[0]);
					level->acidLines.insert(level->acidLines.begin(),&r[1]);
					i++;
				}
			}
		}
		for(int i = 0;i < level->fallLines.size();i++){
			line* r = level->fallLines[i]->splitBy(hit);
			if(r != 0){
				if(level->fallLines[i] == on) on = 0;
				if(r[0].pointA.x == 1000){
					level->fallLines.erase(level->fallLines.begin()+i);
					i--;
				}else if(r[1].pointA.x == 1000){
					level->fallLines.erase(level->fallLines.begin()+i);
					level->fallLines.insert(level->fallLines.begin(),&r[0]);
				}else{
					level->fallLines.erase(level->fallLines.begin()+i);
					level->fallLines.insert(level->fallLines.begin(),&r[0]);
					level->fallLines.insert(level->fallLines.begin(),&r[1]);
					i++;
				}
			}
		}
		if((acidCent.x-level->goal.x)*(acidCent.x-level->goal.x)+(acidCent.y-level->goal.y)*(acidCent.y-level->goal.y) < 2500){
			return 1;
		}
	}
	if(hitBox.y < 0 || hitBox.x < 0 || hitBox.x > 640){
		return 2;
	}
	return 0;
}
bool player::spewAcid(){
	if(acidDur < 75) return false; //HEY! You can't rapid-fire acid!
	acidDur = 0;
	acidDir.clear();
	for(int i = 0;i < 100;i++){
		acidDir.push_back((rand()%4096)/2048.f*PI);
	}
	acidCent.x = hitBox.x;
	acidCent.y = hitBox.y;
}
bool player::checkHit(line* b){
	if(b == on) return false;
	float m = (b->pointA.y-b->pointB.y)/(b->pointA.x-b->pointB.x);
	float q = b->pointA.y + (m * (hitBox.x - b->pointA.x)) - hitBox.y;
	float r = hitBox.r;
	float top = -q*q+m*m*r*r+r*r;
	if(top <= 0) return false;
	r = r + 0.01; //Margin of error fer the floats.
	top = sqrt(top);
	float upperSolution = (top - q*m)/(m*m+1);
	float lowerSolution = (-top - q*m)/(m*m+1);
	if(lowerSolution + hitBox.x > b->pointB.x) return false;
	if(upperSolution + hitBox.x < b->pointA.x) return false;
	float solS = sqrt((m*m+1)*r*r);
	if(q < 0) solS = -solS;
	float necMove = q-solS;
	if(m == 0){
		if(on == 0 || necMove > 0){
			hitBox.y += necMove;
		}else{
			hitBox.y += necMove;
			hitBox.x += necMove/((on->pointA.y-on->pointB.y)/(on->pointA.x-on->pointB.x));
		}
	}else{
		if(on == 0 || necMove > 0){
			float xMoveCost = -m - 1/m;
			float xC = necMove/xMoveCost;
			if(xC > 0){
				float q = 55;
			}
			hitBox.x += xC;
			hitBox.y += -1/m*xC;
		}else{
			float onSlope = (on->pointA.y-on->pointB.y)/(on->pointA.x-on->pointB.x);
			float xMoveCost = -m + onSlope;
			if(abs(xMoveCost) < 0.001){
				if(b->pointA.x-hitBox.x > hitBox.y - b->pointB.x){
					float xC = -hitBox.r;
					hitBox.x += xC;
					hitBox.y += xC * onSlope;
				}else{
					float xC = hitBox.r;
					hitBox.x += xC;
					hitBox.y += xC * onSlope;
				}
			}else{
				float xC = necMove/xMoveCost;
				hitBox.x += xC;
				hitBox.y += xC * onSlope;
			}
		}
	}
	xSpeed = 0;
	ySpeed = 0;
	if(necMove > 0){
		if(m > 5 || m < -5) return true;
		on = b;
		jumped = true;
	}
	return true;
}
void player::checkHit(int x){
	if(x == 0) return;
	for(int i = 0;i < level->acidLines.size();i++){
		if(checkHit(level->acidLines[i])){
			checkHit(x-1);
			return;
		}
	}
	for(int i = 0;i < level->noAcidLines.size();i++){
		if(checkHit(level->noAcidLines[i])){
			checkHit(x-1);
			return;
		}
	}
	for(int i = 0;i < level->fallingLines.size();i++){
		if(checkHit(level->fallingLines[i])){
			checkHit(x-1);
			return;
		}
	}
	for(int i = 0;i < level->fallLines.size();i++){
		if(checkHit(level->fallLines[i])){
			if(on == level->fallLines[i]){
				float onSlope = (level->fallLines[i]->pointA.y-level->fallLines[i]->pointB.y)/(level->fallLines[i]->pointA.x-level->fallLines[i]->pointB.x);
				float moveCost = sqrt(onSlope*onSlope+1);
				float xMove = RED_FALL/moveCost;
				if(hitBox.x + xMove > level->fallLines[i]->pointB.x){
					if(level->fallLines[i]->pointB.x - 2*xMove < level->fallLines[i]->pointA.x){
						level->fallingLines.push_back(level->fallLines[i]);
						level->fallLines.erase(level->fallLines.begin()+i);
					}else{
						vector2 middle;
						middle.x = level->fallLines[i]->pointB.x - 2*xMove;
						middle.y = level->fallLines[i]->pointB.y - 2*onSlope*xMove;
						line* lll = new line(middle,level->fallLines[i]->pointB);
						level->fallLines[i]->pointB = middle;
						on = lll;
						level->fallingLines.push_back(lll);
					}
				}else if(hitBox.x - xMove < level->fallLines[i]->pointA.x){
					if(level->fallLines[i]->pointA.x + 2*xMove > level->fallLines[i]->pointB.x){
						level->fallingLines.push_back(level->fallLines[i]);
						level->fallLines.erase(level->fallLines.begin()+i);
					}else{
						vector2 middle;
						middle.x = level->fallLines[i]->pointA.x + 2*xMove;
						middle.y = level->fallLines[i]->pointA.y + 2*onSlope*xMove;
						line* lll = new line(level->fallLines[i]->pointA,middle);
						level->fallLines[i]->pointA = middle;
						on = lll;
						level->fallingLines.push_back(lll);
					}
				}else{
					vector2 left;
					left.x = hitBox.x - xMove;
					left.y = level->fallLines[i]->pointA.y + ((hitBox.x-xMove-level->fallLines[i]->pointA.x)*onSlope);
					vector2 right;
					right.x = hitBox.x + xMove;
					right.y = level->fallLines[i]->pointA.y + ((hitBox.x+xMove-level->fallLines[i]->pointA.x)*onSlope);
					level->fallLines.push_back(new line(right,level->fallLines[i]->pointB));
					level->fallLines[i]->pointB = left;
					line* lll = new line(left,right);
					on = lll;
					level->fallingLines.push_back(lll);
				}
			}
			return;
		}
	}
}
void player::goNowhere(){
	if(on == 0){
		ang = PI;
		ySpeed -= PLAYER_GRAVITY;
		ySpeed *= PLAYER_SLOWDOWN;
		xSpeed *= PLAYER_SLOWDOWN;
		hitBox.x += xSpeed;
		hitBox.y += ySpeed;
	}else{
		float lineAng = atan2(on->pointA.y-on->pointB.y,on->pointA.x-on->pointB.x) + PI;
		if(lineAng > PI){
			lineAng -= PI;
			ang = lineAng;
		}else{
			ang = lineAng+PI;
		}
		float m = (on->pointA.y-on->pointB.y)/(on->pointA.x-on->pointB.x);
		float nang = lineAng/PI*2-1;
		if(nang < 0) nang = -nang;
		float cost = sqrt(m*m+1);
		float cost2 = sqrt(1/(m*m) + 1);
		if(m!=0){
		ySpeed -= PLAYER_GRAVITY/cost2*(1-nang);
		xSpeed = ySpeed/m;
		}
		float stoppingPower = PLAYER_ACC_X/cost*nang;
		if(xSpeed <= stoppingPower && xSpeed >= -stoppingPower){
			xSpeed = 0;
			ySpeed = 0;
		}
		if(xSpeed > stoppingPower){
			xSpeed -= stoppingPower;
			ySpeed = xSpeed*m;
		}
		if(xSpeed < -stoppingPower){
			xSpeed += stoppingPower;
			ySpeed = xSpeed*m;
		}
		ySpeed *= PLAYER_SLOWDOWN;
		xSpeed *= PLAYER_SLOWDOWN;
		hitBox.x += xSpeed;
		hitBox.y += ySpeed;
		float fac = sqrt(m*m+1);
		if(on == 0 || hitBox.x+hitBox.r/fac < on->pointA.x || hitBox.x-hitBox.r/fac > on->pointB.x)
			on = 0;
	}
	checkHit();
}
void player::goLeft(){
	if(on == 0){
		ang = PI;
		ySpeed -= PLAYER_GRAVITY;
		xSpeed -= PLAYER_ACC_X*PLAYER_AIR_PENALTY;
		ySpeed *= PLAYER_SLOWDOWN;
		xSpeed *= PLAYER_SLOWDOWN;
		hitBox.x += xSpeed;
		hitBox.y += ySpeed;
	}else{
		float lineAng = atan2(on->pointA.y-on->pointB.y,on->pointA.x-on->pointB.x) + PI;
		if(lineAng > PI){
			lineAng -= PI;
			ang = lineAng;
		}else{
			ang = lineAng+PI;
		}
		float m = (on->pointA.y-on->pointB.y)/(on->pointA.x-on->pointB.x);
		float nang = lineAng/PI*2-1;
		if(nang < 0) nang = -nang;
		if(m < 0){
			float cost = sqrt(m*m+1);
			float cost2 = sqrt(1/(m*m) + 1);
			xSpeed -= PLAYER_ACC_X/cost*nang;
			ySpeed = xSpeed*m;
			ySpeed -= PLAYER_GRAVITY/cost2*(1-nang);
			xSpeed = ySpeed/m;
			ySpeed *= PLAYER_SLOWDOWN;
			xSpeed *= PLAYER_SLOWDOWN;
			hitBox.x += xSpeed;
			hitBox.y += ySpeed;
		}else{
			xSpeed -= PLAYER_ACC_X;
			ySpeed -= PLAYER_GRAVITY;
			if(xSpeed*m > ySpeed)
				ySpeed = xSpeed*m;
			else 
				on = 0;
			xSpeed *= PLAYER_SLOWDOWN;
			ySpeed *= PLAYER_SLOWDOWN;
			hitBox.x += xSpeed;
			hitBox.y += ySpeed;
		}
		float fac = sqrt(m*m+1);
		if(on == 0 || hitBox.x+hitBox.r/fac < on->pointA.x || hitBox.x-hitBox.r/fac > on->pointB.x)
			on = 0;
	}
	checkHit();
}
void player::goRight(){
	if(on == 0){
		ang = PI;
		ySpeed -= PLAYER_GRAVITY;
		xSpeed += PLAYER_ACC_X*PLAYER_AIR_PENALTY;;
		ySpeed *= PLAYER_SLOWDOWN;
		xSpeed *= PLAYER_SLOWDOWN;
		hitBox.x += xSpeed;
		hitBox.y += ySpeed;
	}else{
		float lineAng = atan2(on->pointA.y-on->pointB.y,on->pointA.x-on->pointB.x) + PI;
		if(lineAng > PI){
			lineAng -= PI;
			ang = lineAng;
		}else{
			ang = lineAng + PI;
		}
		float m = (on->pointA.y-on->pointB.y)/(on->pointA.x-on->pointB.x);
		float nang = lineAng/PI*2-1;
		if(nang < 0) nang = -nang;
		if(m > 0){
			float cost = sqrt(m*m+1);
			float cost2 = sqrt(1/(m*m) + 1);
			xSpeed += PLAYER_ACC_X/cost*nang;
			ySpeed = xSpeed*m;
			ySpeed -= PLAYER_GRAVITY/cost2*(1-nang);
			xSpeed = ySpeed/m;
			ySpeed *= PLAYER_SLOWDOWN;
			xSpeed *= PLAYER_SLOWDOWN;
			hitBox.x += xSpeed;
			hitBox.y += ySpeed;
		}else{
			xSpeed += PLAYER_ACC_X;
			ySpeed -= PLAYER_GRAVITY;
			if(xSpeed*m > ySpeed)
				ySpeed = xSpeed*m;
			else 
				on = 0;
			xSpeed *= PLAYER_SLOWDOWN;
			ySpeed *= PLAYER_SLOWDOWN;
			hitBox.x += xSpeed;
			hitBox.y += ySpeed;
		}
		float fac = sqrt(m*m+1);
		if(on == 0 || hitBox.x+hitBox.r/fac < on->pointA.x || hitBox.x-hitBox.r/fac > on->pointB.x)
			on = 0;
	}
	checkHit();
}
void player::goUp(){
	if(on != 0){
		on = 0; 
		ySpeed += 10;
		jumped = true;
		return;
	}
	if(jumped){
		ySpeed += 10;
		jumped = false;
	}
}
void player::render(imageManager* mgr){
	glPushMatrix();
	mgr->x[plyr_sprite].Bind();
	glTranslatef(hitBox.x,hitBox.y,0.f);
	glRotatef(ang/PI*180, 0.f, 0.f, 1.f);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.f,0.f);
	glVertex2f(-hitBox.r,-hitBox.r);
	glTexCoord2f(1.f,0.f);
	glVertex2f(hitBox.r,-hitBox.r);
	glTexCoord2f(1.f,1.f);
	glVertex2f(hitBox.r,hitBox.r);
	glTexCoord2f(0.f,1.f);
	glVertex2f(-hitBox.r,hitBox.r);

	glEnd();
	glPopMatrix();
	level->render(mgr);
	glBindTexture(GL_TEXTURE_2D, 0);

	if(acidDur < 25){
		glPushMatrix();
		glTranslatef(acidCent.x, acidCent.y, 0.f);
		for(int i = 0;i < acidDir.size();i++){
			glPushMatrix();
			glTranslatef(cos(acidDir[i])*acidDur*2,sin(acidDir[i])*acidDur*2,0.f);
			glRotatef(acidDur*8, 0.f, 0.f, 1.f);
			glScalef(5-acidDur/5,5-acidDur/5,5-acidDur/5);
			glColor3f(0.f,1.f,0.f);
			glBegin(GL_QUADS);

			glTexCoord2f(0.f,0.f);
			glVertex2f(-1.f,-1.f);
			glTexCoord2f(1.f,0.f);
			glVertex2f(1.f,-1.f);
			glTexCoord2f(1.f,1.f);
			glVertex2f(1.f,1.f);
			glTexCoord2f(0.f,1.f);
			glVertex2f(-1.f,1.f);
			
			glEnd();
			glPopMatrix();
		}
		glPopMatrix();
	}
}