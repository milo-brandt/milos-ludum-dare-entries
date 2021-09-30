//
//  player.cpp
//  StructureSmash
//
//  Created by Milo Brandt on 2/22/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "player.h"
#include "gameInstance.h"
#include <math.h>

player::player():underx(INT_MAX),undery(INT_MAX){}
bool player::collidesWith(gameInstance * g, int x, int y){
    cell c = g->g->getCell(x, y);
    vector2 rel(pos.x-x,pos.y-y);
    if(c.col == cell::NONE) return false;
    float m = c.linM();
    float b = c.linB();
    return rel.x>-0.1&&rel.x<1.1&&rel.y>-0.66&&rel.y<m*fmaxf(0,fminf(rel.x+(m>0?.1:-.1), 1))+b;
}
vector2 player::correctOneCollision(gameInstance * g){
    cell touching;
    int xb = floor(pos.x);
    int yb = floor(pos.y);
    g->g->ensureRect(xb-1, yb, xb+1, yb+1);
    if(collidesWith(g, xb, yb)){
        touching = g->g->getCell(xb, yb);
    }else if(collidesWith(g, xb, yb+1)){
        touching = g->g->getCell(xb, yb+1);
        yb=yb+1;
    }else if(collidesWith(g, xb-1, yb)){
        touching = g->g->getCell(xb-1, yb);
        xb=xb-1;
    }else if(collidesWith(g, xb-1, yb+1)){
        touching = g->g->getCell(xb-1, yb+1);
        xb=xb-1;
        yb=yb+1;
    }else if(collidesWith(g, xb+1, yb)){
        touching = g->g->getCell(xb+1, yb);
        xb=xb+1;
    }else if(collidesWith(g, xb+1, yb+1)){
        touching = g->g->getCell(xb+1, yb+1);
        xb=xb+1;
        yb=yb+1;
    }else{
        xb=INT_MAX;
        yb=INT_MAX;
    }
    if(touching.material == 0) return vector2();
    return correctCollision(g, xb, yb);
}
vector2 player::correctCollision(gameInstance * g, int x, int y){
    cell c = g->g->getCell(x, y);
    vector2 rel(pos.x-x,pos.y-y);
    if(c.col == cell::NONE) return vector2();
    float m = c.linM();
    float b = c.linB();
    float bT = 1000000;
    vector2 norm;
    vector2 topNorm = vector2(-m,1).unit();
    if(topNorm.dot(rel)-fabsf(topNorm.x)*.1<b*topNorm.y && topNorm.dot(veloc) < 0 && (c.col != cell::SQUARE || g->g->getCell(x, y+1).col==cell::NONE)){
        bT = (topNorm.dot(rel)-fabsf(topNorm.x)*.1-b*topNorm.y)/topNorm.dot(veloc);
        norm = topNorm;
    }
    if((g->g->getCell(x-1, y).col == cell::NONE || g->g->getCell(x-1, y).linM() < 0) && b > 0.7 && rel.x > -.1 && veloc.x > 0){
        float bTn = (rel.x + .1)/veloc.x;
        if(bTn < bT){
            bT = bTn;
            norm = vector2(-1,0);
        }
    }
    if((g->g->getCell(x+1, y).col == cell::NONE || g->g->getCell(x+1, y).linB() < 0.8) && b+m > 0.7 && rel.x < 1.1 && veloc.x < 0){
        float bTn = (rel.x - 1.1)/veloc.x;
        if(bTn < bT){
            bT = bTn;
            norm = vector2(1,0);
        }
    }
    if(g->g->getCell(x, y-1).col == cell::NONE &&rel.y > -0.66 && veloc.y > 0){
        float bTn = (rel.y + 0.66)/veloc.y;
        if(bTn < bT){
            bT = bTn;
            norm = vector2(0,-1);
        }
    }
    if(norm.magnitudeSq()>0.5){
        vector2 pvel = veloc.dot(norm)*norm;
        pos -= pvel*(bT+.001/pvel.magnitude());
        if(veloc.dot(norm)<0){
            veloc -= pvel;
        }
        if(norm.y > 0){
            underx = x;
            undery = y;
        }
        return norm;
    }
    return vector2();
}
void player::move(gameInstance* g,float dt){
    cell under;
    if(underx != INT_MAX){
        under = g->g->getCell(underx, undery);
        if(under.col == cell::NONE || pos.y - under.linM()*pos.x - fabsf(under.linM())*.1 > 0.02 + undery + under.linB() - underx*under.linM() || pos.y > undery + 1.02 || fabsf(pos.x - underx - 0.5)>=0.6){
            bool found = false;
            g->g->ensureRect(underx-1, undery-1, underx+1, undery+1);
            for(int i = 0;i<8;i++){
                if(i==0) ++underx;
                if(i==1) ++undery;
                if(i==2) --underx;
                if(i==3) --underx;
                if(i==4) --undery;
                if(i==5) --undery;
                if(i==6) ++underx;
                if(i==7) ++underx;
                under = g->g->getCell(underx, undery);
                if(under.col != cell::NONE && pos.y - under.linM()*pos.x - fabsf(under.linM())*.1 < 0.02 + undery + under.linB() - underx*under.linM() && pos.y < undery + 1.02 && fabsf(pos.x - underx - 0.5)<0.6){
                    found = true;
                    break;
                }
            }
            if(!found){
                under = cell();
                underx = INT_MAX;
            }
        }
    }
    float gravity = 3; //Units are fun. No units are funner. This is gravity.
    bool hit = false;
    while(correctOneCollision(g).magnitudeSq() > 0.1){hit = true;}
    if(!hit && under.material == 0){ //flying
        pos += dt*dt*vector2(0,-gravity);
        veloc.y -= gravity*dt;
        float maxMove = 0.2;
        float vNow = veloc.x;
        float dif = wantMove-vNow;
        veloc.x+=fmaxf(-maxMove*dt,fminf(maxMove*dt, dif));
    }else{
        vector2 parr(1,under.linM());
        parr = parr.unit();
        vector2 norm = parr.rotR();
        veloc-=parr*(parr.y*gravity)*dt;
        float maxMove = under.getSFriction()*norm.y*gravity;
        float vNow = parr.dot(veloc);
        float dif = wantMove-vNow;
        veloc+=fmaxf(-maxMove*dt,fminf(maxMove*dt, dif))*parr;
        if(wantJump){
            veloc += 3*norm+vector2(0,2);
            wantJump = false;
        }
    }
    while(correctOneCollision(g).magnitudeSq() > 0.1){}
    if(veloc.magnitudeSq() > (1/dt/dt)){
        veloc = veloc.unit()/dt;
    }
    for(int i = 0;i< 10;++i){
        pos += veloc*dt/10;
        while(correctOneCollision(g).magnitudeSq() > 0.1){}
        if(wantJump && underx != INT_MAX){
            under = g->g->getCell(underx, undery);
            vector2 parr(1,under.linM());
            parr = parr.unit();
            vector2 norm = parr.rotR();
            
            veloc += 3*norm+vector2(0,2);
            wantJump = false;
        }
    }

    //wantJump = false;
}