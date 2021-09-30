//
//  gameState.cpp
//  Mississippi10
//
//  Created by Milo Brandt on 8/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "gameState.h"
#include "enemy.h"
gameState::gameState(int w,int h,int frames,float timeperframe,float nRad,vector2 gravity):w(w),h(h),frames(frames),frame(0),nRad(nRad),gravity(gravity),timeperframe(timeperframe),selected(0),win(false){
}
void gameState::colCheck(ninja &n){
    for(rectangle const& r : staticRect) colCheck(n,r);
    for(rectangle* const& r : movingRect) colCheck(n,r[frame]);
    for(rectangle const& r : spikes) if(colCheck(n,r)){n.veloc[frame] =vector2();n.pos[frame].y = r.upLeft.y+.2;}
}
bool gameState::canJump(ninja& n){
    for(rectangle const& r : staticRect)
        if(r.upLeft.x-nRad < n.pos[frame].x && r.downRight.x+nRad > n.pos[frame].x && r.upLeft.y-nRad-.2 < n.pos[frame].y && r.downRight.y+nRad > n.pos[frame].y) return true;
    for(rectangle* const& r : movingRect)
        if(r[frame].upLeft.x-nRad < n.pos[frame].x && r[frame].downRight.x+nRad > n.pos[frame].x && r[frame].upLeft.y-nRad-.2 < n.pos[frame].y && r[frame].downRight.y+nRad > n.pos[frame].y) return true;
    return false;
}
bool gameState::spiked(ninja& n){
    for(rectangle const& r : spikes)
        if(r.upLeft.x-nRad < n.pos[frame].x && r.downRight.x+nRad > n.pos[frame].x && r.upLeft.y-nRad-.2 < n.pos[frame].y && r.downRight.y+nRad > n.pos[frame].y){n.pos[frame].y = r.upLeft.y+.2; return true;}
    return false;
}

bool gameState::colCheck(ninja& n,rectangle const& r){
    if(r.upLeft.x-nRad < n.pos[frame].x && r.downRight.x+nRad > n.pos[frame].x && r.upLeft.y-nRad < n.pos[frame].y && r.downRight.y+nRad > n.pos[frame].y){
        float xTime = 100000000;
        float yTime = 100000000;
        float xnTime = 10000000;
        float ynTime = 10000000;
        if(n.veloc[frame].x > 0.01){
            xTime = (n.pos[frame].x - r.upLeft.x + nRad)/n.veloc[frame].x;
        }else if(n.veloc[frame].x < -0.01){
            xTime = (n.pos[frame].x - r.downRight.x - nRad)/n.veloc[frame].x;
        }
        if(n.veloc[frame].y > 0.01){
            yTime = (n.pos[frame].y - r.upLeft.y + nRad)/n.veloc[frame].y;
        }else if(n.veloc[frame].y < -0.01){
            yTime = (n.pos[frame].y - r.downRight.y - nRad)/n.veloc[frame].y;
        }
            if(n.pos[frame].x - r.upLeft.x < r.downRight.x - n.pos[frame].x) xnTime = r.upLeft.x-nRad-n.pos[frame].x;
            else xnTime=r.downRight.x+nRad-n.pos[frame].x;
            if(n.pos[frame].y - r.upLeft.y < r.downRight.y - n.pos[frame].y) ynTime = r.upLeft.y-nRad-n.pos[frame].y;
            else ynTime=r.downRight.y+nRad-n.pos[frame].y;
        float nmin = xTime;
        if(xTime > yTime) nmin = yTime;
        float min = abs(xnTime);
        if(abs(xnTime) > abs(ynTime)) nmin = abs(ynTime);
        
        if(true||min*timeperframe*0<nmin){
            if(fabs(xnTime) < fabs(ynTime)){
                n.pos[frame].x += xnTime;
                n.veloc[frame].x = 0;
            }else{
                n.pos[frame].y += ynTime;
                n.veloc[frame].y = 0;
            }
        }else{
            n.pos[frame] -= n.veloc[frame]*(xTime<yTime?xTime:yTime);
            if(xTime<yTime)
                n.veloc[frame].x = 0;
            else
                n.veloc[frame].y = 0;
        }
        return true;
    }
    return false;
}
vector2 gameState::shurikenDemise(vector2 origin, vector2 target,int t){
    float destroyedTime = 10000000;
    vector2 dir = target - origin;
    for(rectangle& r : staticRect){
        float k = hitTime(origin, dir, r);
        if(k < destroyedTime) destroyedTime = k;
    }
    for(rectangle* r : movingRect){
        float k = hitTime(origin, dir, r[t]);
        if(k < destroyedTime) destroyedTime = k;
    }
    return origin + dir * destroyedTime;
}

void gameState::fireShuriken(vector2 origin, vector2 target){
    float destroyedTime = 10000000;
    vector2 dir = target - origin;
    for(rectangle& r : staticRect){
        float k = hitTime(origin, dir, r);
        if(k < destroyedTime) destroyedTime = k;
    }
    for(rectangle* r : movingRect){
        float k = hitTime(origin, dir, r[frame]);
        if(k < destroyedTime) destroyedTime = k;
    }
    for(enemy* r : enemies){
        float k = hitTime(origin, dir, r->positions[frame]);
        if(k < destroyedTime){
            if(frame < r->killFrame){
                r->onDeath(frame, this);
                r->killFrame = frame;
            }
        }
    }
}
float gameState::hitTime(vector2 origin,vector2 dir,rectangle r){
    float minYT = 0;
    float maxYT = 10000;
    float minXT = 0;
    float maxXT = 10000;
    if(dir.y != 0){
        minYT = (r.upLeft.y-origin.y)/dir.y;
        maxYT = (r.downRight.y-origin.y)/dir.y;
        if(minYT > maxYT) std::swap(minYT, maxYT);
    }
    if(dir.x != 0){
        minXT = (r.upLeft.x-origin.x)/dir.x;
        maxXT = (r.downRight.x-origin.x)/dir.x;
        if(minXT > maxXT) std::swap(minXT, maxXT);
    }
    if(maxYT < minXT) return 10000000;
    if(maxXT < minYT) return 10000000;
    if(maxXT < 0 || maxYT < 0) return 10000000;
    if(minYT > minXT) return minYT;
    return minXT;
}
void gameState::update(){
    win = true;
        for(enemy* e : enemies){
            if(e->critical && e->killFrame > frame) win = false;
        }
        for(ninja& n : agents){
            if(n.pos[frame].x > 0 && n.pos[frame].x < w && n.pos[frame].y > 0 && n.pos[frame].y < h){
                win= false;
            }
        }
    if(frame+1 == frames){return;}
    ++frame;
    for(ninja& n : agents){
        n.veloc[frame] = n.veloc[frame-1] + gravity * timeperframe;
        if(n.movAction[frame] == 'W') n.veloc[frame].y = -5; //Jump;
        if(n.movAction[frame] == 'A') n.veloc[frame].x -= 5 * timeperframe;
        if(n.movAction[frame] == 'D') n.veloc[frame].x += 5 * timeperframe;
        n.pos[frame] = n.pos[frame-1];
        bool spike = spiked(n);
        if(spike)
            n.veloc[frame] = 0;
        n.pos[frame] = n.pos[frame-1] + n.veloc[frame]*timeperframe;
        if(n.shurikenTime == frame && !spike){
            fireShuriken(n.pos[frame], n.shurikenTarg);
        }
        if(!spike) colCheck(n);
    }
}