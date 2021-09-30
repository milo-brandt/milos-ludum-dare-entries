//
//  grid.h
//  StructureSmash
//
//  Created by Milo Brandt on 2/22/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef StructureSmash_grid_h
#define StructureSmash_grid_h

#include <deque>
#include "vector2.h"


//0: Nothing
//1: Snow
//2: Earth
class cell{
public:
    enum collisionBox{NONE,SQUARE,SLOPE_1_R,SLOPE_HALF_TOP_R,SLOPE_HALF_BOTTOM_R,SLOPE_1_L,SLOPE_HALF_TOP_L,SLOPE_HALF_BOTTOM_L};
    collisionBox col;
    int material;
    cell();
    cell(collisionBox col,int material);
    float linM() const;
    float linB() const;
    float getSFriction() const;
    float getKFriction() const;
};
class grid{
    std::deque<std::pair<std::deque<cell>*,int>> internal;
    int zeroX;
public:
    class iterator{
        int x1;
        int y1;
        int x2;
        int y2;
        int x,y;
        grid* g;
        iterator();
        iterator(grid* g);
        iterator(grid* g,int x1,int y1,int x2,int y2);
    public:
        std::pair<cell&, vector2> operator*() const;
        iterator& operator++();
        bool operator==(iterator const& b)const;
        bool operator!=(iterator const& b)const;
        friend grid;
    };
    friend grid::iterator;
    grid();
    int getFirstBlock(int x);
    cell& getCell(int x,int y);
    void ensurePoint(int x,int y);
    void ensureRect(int x1,int y1,int x2,int y2);
    iterator beginRect(int x1,int y1,int x2,int y2);
    iterator begin();
    iterator end();
    class subgrid{
        int x1;
        int y1;
        int x2;
        int y2;
        grid* g;
        subgrid(grid* g, int x1,int y1,int x2,int y2);
        friend grid;
    public:
        iterator begin();
        iterator end();
        void ensure();
    };
    subgrid subGrid(int x1,int y1,int x2,int y2);
    std::pair<vector2, vector2> bounds();
    ~grid();
};

#endif
