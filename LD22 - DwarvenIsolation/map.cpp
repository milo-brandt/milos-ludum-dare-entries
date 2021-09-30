//
//  map.cpp
//  DwarvenIsolation
//
//  Created by Milo Brandt on 12/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "map.h"
#include <math.h>
#include <queue>
#define screamDist 24
#define screamSpeed 4
#define screamAlpha 0.4f

position::position(int x,int y):x(x),y(y){}
place::place(placeType t):t(t),d(0),seendegree(0){}
place* place::greatestValue(map* m){
    place* best = this;
    unsigned int v = this->value();
    place* n = m->getAt(position(pos.x+1,pos.y));
    unsigned int q = n->value();
    if(q > v){
        best = n;
        v = q;
    }
    n = m->getAt(position(pos.x-1,pos.y));
    q = n->value();
    if(q > v){
        best = n;
        v = q;
    }
    n = m->getAt(position(pos.x,pos.y+1));
    q = n->value();
    if(q > v){
        best = n;
        v = q;
    }
    n = m->getAt(position(pos.x,pos.y-1));
    q = n->value();
    if(q > v){
        best = n;
        v = q;
    }
    return best;
}
unsigned int place::value(){ return 0; }
unsigned int place::walk(){ return -1; }
void place::onRemove(map *m,dwarf* d){}
bool place::see(){ return false; }
void place::render(sf::RenderWindow &w, imageManager &m){
    w.Draw(sf::Sprite(m.imgs[m.unbreakable],sf::Vector2f(pos.x*10,pos.y*10)));
}
unsigned int empty::value(){ return 0; }
unsigned empty::walk(){ return 5; }
bool empty::see(){ return true; }
void empty::onRemove(map *m,dwarf* d){}
void empty::render(sf::RenderWindow &w, imageManager &m){
    w.Draw(sf::Shape::Rectangle(pos.x*10,pos.y*10,pos.x*10+10, pos.y*10+10,seendegree>0?sf::Color(64,64,48):sf::Color(0,0,0)));
}
unsigned int deadDwarf::value(){ return -1; } //HUGE VALUE!
unsigned deadDwarf::walk(){ return 5; }
bool deadDwarf::see(){ return true; }
void deadDwarf::onRemove(map *m,dwarf* dw){
    m->soundNum = 4;
    ++m->screams;
    for(std::list<dwarf*>::iterator d = m->dwarves.begin();d != m->dwarves.end();++d){
        if(*d == dw) continue;
        if(!(*d)->alive) continue;
        if(m->dist((*d)->pos->pos,pos) < screamDist){
            delete (*d)->moveTarget;
            (*d)->moveTarget = m->pathfind((*d)->pos->pos, pos);
        }
    }
    scream n;
    n.x = 0;
    n.c = pos;
    n.r = 1;
    n.p = screamDist;
    m->screamList.push_back(n);
}
void deadDwarf::render(sf::RenderWindow &w, imageManager &m){
    w.Draw(sf::Sprite(m.imgs[m.dead],sf::Vector2f(pos.x*10,pos.y*10)));
}
unsigned int stickyTrap::value(){ return 0; }
unsigned stickyTrap::walk(){ return 5; }
bool stickyTrap::see(){ return true; }
void stickyTrap::onRemove(map *m,dwarf* d){
    m->soundNum = 0;
    d->moveProgress = -500; //Restrain for 500 ticks
}
void stickyTrap::render(sf::RenderWindow &w, imageManager &m){
    w.Draw(sf::Sprite(m.imgs[m.sticky],sf::Vector2f(pos.x*10,pos.y*10)));
    if(seendegree > 0){
        w.Draw(sf::Shape::Rectangle(pos.x*10,pos.y*10,pos.x*10+10, pos.y*10+10,sf::Color(255,255,192,64)));
    }
}
unsigned int killTrap::value(){ return 0; }
unsigned killTrap::walk(){ return 5; }
bool killTrap::see(){ return true; }
void killTrap::onRemove(map *m,dwarf* d){
    m->soundNum = 2;
    d->kill(); //OH NO!
    ++m->kills;
}
void killTrap::render(sf::RenderWindow &w, imageManager &m){
    w.Draw(sf::Sprite(m.imgs[m.deadly],sf::Vector2f(pos.x*10,pos.y*10)));
    if(seendegree > 0){
        w.Draw(sf::Shape::Rectangle(pos.x*10,pos.y*10,pos.x*10+10, pos.y*10+10,sf::Color(255,255,192,64)));
    }
}
rock::rock(unsigned int mineTime,unsigned int value):place(place::ROCK),mineTime(mineTime),valu(value){}
unsigned int rock::value(){ return valu; }
unsigned int rock::walk(){ return mineTime; }
bool rock::see(){ return false; }
void rock::onRemove(map *m,dwarf* d){
    m->money += valu;
    m->score += valu;
}
void rock::render(sf::RenderWindow &w, imageManager &m){
    w.Draw(sf::Sprite(m.imgFromValue(valu),sf::Vector2f(pos.x*10,pos.y*10)));
}
unsigned int shiny::value(){ return -2; }
unsigned int shiny::walk(){ return 5; }
bool shiny::see(){ return true; }
void shiny::onRemove(map *m,dwarf* d){
    m->soundNum = 1;
}
void shiny::render(sf::RenderWindow &w, imageManager &m){
    w.Draw(sf::Sprite(m.imgs[m.shinyCoin],sf::Vector2f(pos.x*10,pos.y*10)));
    if(seendegree > 0){
        w.Draw(sf::Shape::Rectangle(pos.x*10,pos.y*10,pos.x*10+10, pos.y*10+10,sf::Color(255,255,192,64)));
    }
}
map::map(unsigned int width,unsigned int height):placeArray(new place*[width*height]),width(width),height(height),loser(0),score(0),money(0),screams(0),kills(0),soundNum(-1){
    unsigned int i = 0;
    for(unsigned int y = 0;y < height;++y){
        for(unsigned int x = 0;x< width;++x){
            placeArray[i] = new place;
            placeArray[i]->pos = position(x,y);
            ++i;
        }
    }
}
namespace{
    void tryDraw(map* m,position n,sf::RenderWindow &r, imageManager &q){
        if(!m->legal(n.x, n.y)) return;
        r.Draw(sf::Shape::Rectangle(n.x*10,n.y*10,n.x*10+10, n.y*10+10,sf::Color(255,255,255,255*screamAlpha)));
        m->needsUpdate.push_back(n);
    }
}
bool scream::render(map* q,sf::RenderWindow &g, imageManager &m){
    if(++x == screamSpeed){
        x = 0;
        if(++r > p) return true;
    }
    tryDraw(q, position(c.x + r,c.y), g, m);
    tryDraw(q, position(c.x - r,c.y), g, m);
    tryDraw(q, position(c.x,c.y + r), g, m);
    tryDraw(q, position(c.x,c.y - r), g, m);
    for(unsigned int i = 1;i < r;++i){
        unsigned int y = r - i;
        tryDraw(q, position(c.x + i,c.y + y), g, m);
        tryDraw(q, position(c.x - i,c.y + y), g, m);
        tryDraw(q, position(c.x + i,c.y - y), g, m);
        tryDraw(q, position(c.x - i,c.y - y), g, m);
    }
    return false;
}
place* map::getAt(position a){
    return placeArray[getPos(a.x, a.y)];
}
void map::setAt(position a, place *r){
    unsigned int x = getPos(a.x,a.y);
    r->pos = a;
    r->seendegree = placeArray[x]->seendegree;
    delete placeArray[x];
    placeArray[x] = r;
}
bool map::isInSight(position a){
    if(getAt(a)->seendegree > 0) return true;
    if(getAt(position(a.x+1,a.y))->seendegree > 0) return true;
    if(getAt(position(a.x-1,a.y))->seendegree > 0) return true;
    if(getAt(position(a.x,a.y+1))->seendegree > 0) return true;
    if(getAt(position(a.x,a.y-1))->seendegree > 0) return true;
    return false;
}
bool map::sight(position a, position b,int radius){
    if(dist(a, b) > radius) return false;
    if(!getAt(a)->see() || !getAt(b)->see()) return false;
    if(a.x == b.x){
        int max = a.y;
        int min = b.y;
        if(max < min) std::swap(max, min);
        unsigned int p = getPos(a.x, min);
        for(int i = min;i < max;++i){
            if (!placeArray[p]->see()) return false;
            p += width;
        }
        return true;
    }
    if(a.y == b.y){
        int max = a.x;
        int min = b.x;
        if(max < min) std::swap(max, min);
        unsigned int p = getPos(min, a.y);
        for(int i = min;i < max;++i){
            if(!placeArray[p]->see()) return false;
            ++p;
        }
        return true;
    }
    return (sight(position(a.x,a.y), position(a.x,b.y), radius) && sight(position(a.x,b.y), position(b.x,b.y), radius)) || (sight(position(a.x,a.y), position(b.x,a.y), radius) && sight(position(b.x,a.y), position(b.x,b.y), radius));
    
}
bool map::legal(unsigned int x, unsigned int y){
    if(x >= width) return false;
    if(y >= height) return false;
    return true;
}
unsigned int map::dist(position a, position b){
    return ((a.x>b.x)?(a.x-b.x):(b.x-a.x))+((a.y>b.y)?(a.y-b.y):(b.y-a.y));
}
namespace {
    struct aStar{
        position a;
        position p;
        unsigned int vTo;
        unsigned int vHeur;
        aStar(position a,position p,unsigned int vTo,unsigned int vHeur):a(a),p(p),vTo(vTo),vHeur(vHeur){}
    };
    bool comp(aStar const& a,aStar const& b){
        return a.vTo + a.vHeur > b.vTo + b.vHeur;
    }
    void legalMove(map* m,position a,position p,position b,unsigned int vTo,std::priority_queue<aStar,std::vector<aStar>,bool(*)(aStar const&,aStar const&)>& q){
        if(!m->legal(a.x, a.y)) return;
        if(m->getAt(p)->walk() == -1) return;
        q.push(aStar(a,p,vTo,m->dist(a,b)*5));
    }
}
path* map::pathfind(position a, position b){
    position* closed = new position[width*height];
    for(unsigned int i = 0;i<width*height;++i){
        closed[i].x = -1;
    }
    std::priority_queue<aStar,std::vector<aStar>,bool(*)(aStar const&,aStar const&)> open(comp);
    open.push(aStar(a,a,0,0));
    while(!open.empty()){
        aStar c = open.top();
        if(c.a.x == b.x && c.a.y == b.y){
            closed[getPos(c.a.x, c.a.y)] = c.p;
            break;
        }
        open.pop();
        unsigned int np = getPos(c.a.x, c.a.y);
        if(closed[np].x != -1) continue;
        closed[np] = c.p;
        unsigned int cost = placeArray[np]->walk() + c.vTo;
        legalMove(this,position(c.a.x+1,c.a.y),c.a,b,cost,open);
        legalMove(this,position(c.a.x-1,c.a.y),c.a,b,cost,open);
        legalMove(this,position(c.a.x,c.a.y+1),c.a,b,cost,open);
        legalMove(this,position(c.a.x,c.a.y-1),c.a,b,cost,open);
    }
    if(open.empty()){
        delete[] closed;
        return 0;
    }
    path* r = new path();
    position c = b;
    while(c.x != a.x || c.y != a.y){
        r->places.push_front(c);
        c = closed[getPos(c.x, c.y)];
    }
    delete[] closed;
    return r;
}
void map::fullRender(sf::RenderWindow &r, imageManager* m){
    if(soundNum != -1){
        sf::Music& so = m->sounds[soundNum];
        so.Play();
        soundNum = -1;
    }
    needsUpdate.clear();
    for(unsigned int i = 0;i < width*height;++i){
        placeArray[i]->render(r, *m);
    }
    for(std::list<dwarf*>::iterator d = dwarves.begin(); d != dwarves.end();++d){
        (*d)->render(r, m);
    }
    for(std::list<scream>::iterator s = screamList.begin(); s != screamList.end();){
        if(s->render(this, r, *m)){
            screamList.erase(s++);
        }else{
            ++s;
        }
    }
}
void map::render(sf::RenderWindow& r,imageManager& m){
    for(std::list<position>::iterator d = needsUpdate.begin(); d != needsUpdate.end();++d){
        getAt(*d)->render(r, m);
    }
    for(std::list<dwarf*>::iterator d = dwarves.begin(); d != dwarves.end();++d){
        (*d)->render(r, &m);
    }
    for(std::list<scream>::iterator s = screamList.begin(); s != screamList.end();){
        if(s->render(this, r, m)){
            screamList.erase(s++);
        }else{
            ++s;
        }
    }
}
void map::update(){
    for(std::list<dwarf*>::iterator d = dwarves.begin(); d != dwarves.end();++d){
        (*d)->step();
    }
}
map::~map(){
    for(std::list<dwarf*>::iterator i = dwarves.begin();i != dwarves.end();++i){
        delete *i;
    }
    for(unsigned int i = 0;i < width*height;++i){
        delete placeArray[i];
    }
    delete[] placeArray;
}