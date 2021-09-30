//
//  dwarf.h
//  DwarvenIsolation
//
//  Created by Milo Brandt on 12/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef dwarf_H_DEF
#define dwarf_H_DEF
#include "map.h"
#include <set>

/*struct dwarfSight{ //Like regular sight, but dwarven
    place* p;
    bool operator<(dwarfSight const& b) const{
        if(p->pos.x < b.p->pos.x) return true;
        if(p->pos.x > b.p->pos.y) return false;
        if(p->pos.y < b.p->pos.y) return true;
        return false;
    }
    dwarfSight(place* p):p(p){}
};*/
class dwarf{
public:
    bool alive;
    int moveProgress;
    path* moveTarget;
    place* pos;
    map* m;
    std::list<position>* sight;
    dwarf(place* p,map* m);
    void kill();
    void step();
    void updatePosition();
    void newPath();
    void render(sf::RenderWindow& w,imageManager* m);
};

#endif