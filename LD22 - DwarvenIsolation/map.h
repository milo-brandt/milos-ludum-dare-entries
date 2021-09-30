//
//  map.h
//  DwarvenIsolation
//
//  Created by Milo Brandt on 12/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef map_H_DEF
#define map_H_DEF
#include <list>
#include "imageManager.h"

class dwarf;
class position{
public:
    int x;
    int y;
    position(int x = 0,int y = 0);
};
class map;
class place{
public:
    enum placeType{
        UNBREAKABLE, STICKY, DEADLY, EMPTY, DEAD, ROCK, SHINY
    } t;
    place(placeType t = UNBREAKABLE);
    position pos;
    dwarf* d;
    unsigned int seendegree;
    place* greatestValue(map* m);
    virtual unsigned int value();
    virtual unsigned int walk(); //Cost of walking. 5 is minimum
    virtual bool see();
    virtual void render(sf::RenderWindow& r,imageManager& m);
    virtual void onRemove(map* m,dwarf* d);
};
class stickyTrap : public place{
public:
    stickyTrap():place(place::STICKY){}
    virtual unsigned int value();
    virtual unsigned int walk(); //Cost of walking
    virtual bool see();
    virtual void render(sf::RenderWindow& r,imageManager& m);
    virtual void onRemove(map* m,dwarf* d);
};
class killTrap : public place{
public:
    killTrap():place(place::DEADLY){}
    virtual unsigned int value();
    virtual unsigned int walk(); //Cost of walking
    virtual bool see();
    virtual void render(sf::RenderWindow& r,imageManager& m);
    virtual void onRemove(map* m,dwarf* d);
};
class empty : public place{
public:
    empty():place(place::EMPTY){}
    virtual unsigned int value();
    virtual unsigned int walk(); //Cost of walking
    virtual bool see();
    virtual void render(sf::RenderWindow& r,imageManager& m);
    virtual void onRemove(map* m,dwarf* d);
};
class deadDwarf : public place{
public:
    deadDwarf():place(place::DEAD){}
    virtual unsigned int value();
    virtual unsigned int walk(); //Cost of walking
    virtual bool see();
    virtual void render(sf::RenderWindow& r,imageManager& m);
    virtual void onRemove(map* m,dwarf* d);
};
class rock : public place{
public:
    unsigned int mineTime;
    unsigned int valu;
    rock(unsigned int mineTime,unsigned int value);
    virtual unsigned int value();
    virtual unsigned int walk();
    virtual bool see();
    virtual void render(sf::RenderWindow& r,imageManager& m);
    virtual void onRemove(map* m,dwarf* d);
};
class shiny : public place{
public:
    shiny():place(place::SHINY){}
    virtual unsigned int value();
    virtual unsigned int walk(); //Cost of walking
    virtual bool see();
    virtual void render(sf::RenderWindow& r,imageManager& m);
    virtual void onRemove(map* m,dwarf* d);
};
class scream{
public:
    position c;
    unsigned int x;
    unsigned int r;
    unsigned int p;
    bool render(map* q,sf::RenderWindow& r,imageManager& m);
};
class path{
public:
    std::list<position> places;
};
class map{
public:
    unsigned int soundNum;
    std::list<scream> screamList;
    unsigned int kills;
    dwarf* loser; //LOOOOSER
    unsigned int score;
    unsigned int money;
    unsigned int screams;
    std::list<dwarf*> dwarves;
    std::list<position> needsUpdate;
    place** placeArray;
    unsigned int width,height;
    map(unsigned int width,unsigned int height);
    bool legal(unsigned int x,unsigned int y);
    inline unsigned int getPos(unsigned int x,unsigned int y){
        return y*width+x;
    }
    void fullRender(sf::RenderWindow& r,imageManager* m);
    void update();
    void render(sf::RenderWindow& r,imageManager& m);
    bool isInSight(position a);
    unsigned int dist(position a,position b);
    bool sight(position a,position b,int radius);
    place* getAt(position a);
    void setAt(position a,place* r);
    path* pathfind(position a,position b);
    ~map();
};
#include "dwarf.h"

#endif