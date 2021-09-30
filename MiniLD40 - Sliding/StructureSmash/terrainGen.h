//
//  terrainGen.h
//  StructureSmash
//
//  Created by Milo Brandt on 2/23/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef StructureSmash_terrainGen_h
#define StructureSmash_terrainGen_h

#include "vector2.h"

class grid;
class terrainGen{
public:
    vector2 right;
    vector2 left;
    int lastSnow;
    terrainGen(grid* g); //g is empty.
    void generateAt(grid* g,int x,int y);
    void jump(grid* g,vector2& p,bool r);
    void steep(grid* g,vector2& p,bool r,int k);
    void steepUp(grid* g,int k);
    void canyon(grid* g,int w,int d);
    void snow(grid* g,int n);
    void thaw(grid* g);
    void snow(grid* g,int n,int x1,int x2);
    void dirt(grid* g,int n,int x1,int x2);
    void flat(grid* g,int n);
    void destroy(grid* g,int x,int y);
    void collapse(grid* g,int x,int y);
};

#endif
