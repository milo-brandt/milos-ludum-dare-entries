//
//  dwarf.cpp
//  DwarvenIsolation
//
//  Created by Milo Brandt on 12/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "dwarf.h"

#define dwarvenSight 8

dwarf::dwarf(place* p,map* m):pos(p),m(m),alive(true){
    m->getAt(p->pos)->d = this;
    m->dwarves.push_back(this);
    moveTarget = new path;
    sight = new std::list<position>;
    updatePosition();
}
void dwarf::step(){
    if(!alive) return;
    unsigned int x = moveTarget->places.size();
    position posp = moveTarget->places.front();
    place* posc = m->getAt(moveTarget->places.front());
    if(++moveProgress >= (int)posc->walk()){
        moveProgress = 0;
        m->needsUpdate.push_back(pos->pos);
        pos->d = 0;
        pos = m->getAt(moveTarget->places.front());
        pos->onRemove(m,this);
        if(pos->d != 0){
            m->loser = this;
        }
        if(alive){
            empty* e = new empty;
            e->d = this;
            m->setAt(pos->pos, e);
            pos = e;
            moveTarget->places.pop_front();
            updatePosition();
        }else{
            deadDwarf* dw = new deadDwarf;
            m->setAt(pos->pos, dw); //Dead.
        }
    }
}
void dwarf::kill(){
    alive = false;
    std::list<position>* oldSight = sight;
    sight = new std::list<position>();
    for(std::list<position>::iterator i = oldSight->begin();i != oldSight->end();++i){
        if(--m->getAt((*i))->seendegree == 0){
            m->needsUpdate.push_back(*i);
        }
    }
    delete oldSight;
}
void dwarf::updatePosition(){
    std::list<position>* oldSight = sight;
    sight = new std::list<position>();
    for(int x = -dwarvenSight;x <= dwarvenSight;++x){
        int q = dwarvenSight - ((x>0)?x:-x);
        for(int y = -q;y <= q;++y){
            position p(x + pos->pos.x,y + pos->pos.y);
            if(!m->legal(p.x, p.y)) continue;
            if(!m->sight(pos->pos, p, dwarvenSight)) continue;
            place* c = m->getAt(p);
            if(++c->seendegree == 1){
                m->needsUpdate.push_back(c->pos);
            }
            sight->push_back(p);
            if(c->d != 0 && c->pos.x != pos->pos.x && c->pos.y != pos->pos.y){ //UH OH!
                delete moveTarget;
                moveTarget = m->pathfind(pos->pos, c->pos);
            }
        }
    }
    for(std::list<position>::iterator i = oldSight->begin();i != oldSight->end();++i){
        if(--m->getAt((*i))->seendegree == 0){
            m->needsUpdate.push_back(*i);
        }
    }
    delete oldSight;
    unsigned int x = moveTarget->places.size();
    bool asdf = moveTarget->places.empty();
    if(moveTarget->places.empty()){
        delete moveTarget;
        newPath();
    }
}
void dwarf::newPath(){
    place* best = m->getAt(sight->front());
    place* bestC = best;
    unsigned int def = (sight->size()<=1)?0:(rand() % (sight->size()-1));
    unsigned int b = 0;
    for(std::list<position>::iterator i = sight->begin();i != sight->end();++i){
        place* p = m->getAt((*i))->greatestValue(m);
        if(p->pos.x == pos->pos.x && p->pos.y == pos->pos.y) ++def;
        unsigned int x = p->value();
        if(b == 0 && x == 0 && --def == -1){
            --def;
            best = m->getAt(*i);
            bestC = best;
        }
        if(x > b){
            best = m->getAt(*i);
            bestC = p;
            b = x;
        }
    }
    moveTarget = m->pathfind(pos->pos, best->pos);
    unsigned int sz = moveTarget->places.size();
    if(sz == 0){
        int asdfasd = 5;
        int dbfs = 123;
    }
    if(bestC != best){
        moveTarget->places.push_back(bestC->pos);
    }
}
void dwarf::render(sf::RenderWindow& w,imageManager* m){
    if(!alive) return;
    w.Draw(sf::Sprite(m->imgs[m->dwarf],sf::Vector2f(pos->pos.x*10,pos->pos.y*10)));
}