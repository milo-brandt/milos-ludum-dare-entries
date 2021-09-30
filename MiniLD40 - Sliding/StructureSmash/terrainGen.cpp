//
//  terrainGen.cpp
//  StructureSmash
//
//  Created by Milo Brandt on 2/23/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "terrainGen.h"
#include "grid.h"

terrainGen::terrainGen(grid* g):lastSnow(1){
    g->ensureRect(-5, -5, 5, 5);
    for(auto p : *g){
        if(p.second.y < -2){
            p.first.material = 2; //Dirt.
            p.first.col = cell::SQUARE;
        }else{
            p.first.material = 3; //Air
            p.first.col = cell::NONE;
        }
    }
    right = vector2(5,-3);
    left = vector2(-5,-3);
    dirt(g, 2, -5, 5);
    steep(g, left, false, 20);
    jump(g, left, false);
    steep(g, left, false, 5);
    jump(g, right, true);
    canyon(g, 10,5);
    thaw(g);
    snow(g,2);
}
void terrainGen::snow(grid *g,int n,int x1,int x2){
    lastSnow = n;
    for(int x = x1;x<=x2;++x){
        int y = g->getFirstBlock(x);
        if(y == INT_MAX) continue;
        g->ensurePoint(x, y+n);
        for(int i = 1;i < n;++i){
            cell& c = g->getCell(x,y+i);
            c.material = 1;
            c.col = cell::SQUARE;
        }
        cell& c2 = g->getCell(x,y+n);
        c2.material = 1;
        c2.col = g->getCell(x, y).col;
    }
}
void terrainGen::dirt(grid *g,int n,int x1,int x2){
    lastSnow = n;
    for(int x = x1;x<=x2;++x){
        int y = g->getFirstBlock(x);
        if(y == INT_MAX) continue;
        g->ensurePoint(x, y+n);
        for(int i = 1;i < n;++i){
            cell& c = g->getCell(x,y+i);
            c.material = 4;
            c.col = cell::SQUARE;
        }
        cell& c2 = g->getCell(x,y+n);
        c2.material = 4;
        c2.col = g->getCell(x, y).col;
    }
}
void terrainGen::thaw(grid *g){
    for(auto p : *g){
        if(p.first.material == 1){
            p.first.material = 3;
            p.first.col = cell::NONE;
        }
    }
}
void terrainGen::snow(grid *g,int n){
    snow(g,n,left.x,right.x);
}
void terrainGen::destroy(grid *g, int x, int y){
    g->ensurePoint(x, y);
    cell& c = g->getCell(x, y);
    c.material = 3;
    c.col = cell::NONE;
    g->ensurePoint(x, y+1);
    if(g->getCell(x, y+1).material == 1){
        collapse(g, x, y+1);
    }
}
void terrainGen::collapse(grid *g, int x, int y){
    g->ensurePoint(x, y);
    g->ensurePoint(x, y-1);
    if(g->getCell(x, y-1).material == 3){
        g->getCell(x, y-1) = g->getCell(x, y);
        destroy(g, x, y);
        collapse(g, x, y-1);
    }
}
void terrainGen::steepUp(grid* g,int k){
    int xk = right.x+1;
    for(int i = 0;i < k;++i){
        ++right.x;
        g->ensurePoint(right.x, right.y);
        g->ensurePoint(right.x, right.y+1);
        cell& c = g->getCell(right.x, right.y);
        c.material = 2;
        c.col = cell::SQUARE;
        ++right.y;
        cell& c2 = g->getCell(right.x, right.y);
        c2.material = 2;
        c2.col = cell::SLOPE_1_L;
        ++right.y;
    }
    dirt(g,2, xk, right.x);
    snow(g,lastSnow,xk,right.x);
}
void terrainGen::flat(grid *g, int n){
    int xk = right.x+1;
    while(n > 1){
        switch(rand()%4){
            case 0:
                ++right.x;
                ++right.y;
                g->ensureRect(right.x, right.y, right.x+1, right.y);
                g->getCell(right.x, right.y).material = 2;
                g->getCell(right.x, right.y).col = cell::SLOPE_HALF_BOTTOM_L;
                ++right.x;
                g->getCell(right.x, right.y).material = 2;
                g->getCell(right.x, right.y).col = cell::SLOPE_HALF_TOP_L;
                n-=2;
                break;
            case 1:
                ++right.x;
                g->ensureRect(right.x, right.y, right.x+1, right.y);
                g->getCell(right.x, right.y).material = 2;
                g->getCell(right.x, right.y).col = cell::SLOPE_HALF_TOP_R;
                ++right.x;
                g->getCell(right.x, right.y).material = 2;
                g->getCell(right.x, right.y).col = cell::SLOPE_HALF_BOTTOM_R;
                n-=2;
                --right.y;
                break;
            default:
                ++right.x;
                g->ensurePoint(right.x, right.y);
                g->getCell(right.x, right.y).material = 2;
                g->getCell(right.x, right.y).col = cell::SQUARE;
                break;
        }
    }
    ++right.x;
    g->ensurePoint(right.x, right.y);
    g->getCell(right.x, right.y).material = 2;
    g->getCell(right.x, right.y).col = cell::SQUARE;
    dirt(g,2,xk,right.x);
    snow(g,lastSnow,xk,right.x);
}
void terrainGen::steep(grid* g,vector2& p,bool r,int k){
    int inc = r?1:-1;
    int xk = p.x+inc;
    for(int i = 0;i < k;++i){
        p.x += inc;
        g->ensurePoint(p.x, p.y);
        g->ensurePoint(p.x, p.y-1);
        cell& c = g->getCell(p.x, p.y);
        c.material = 2;
        c.col = r?cell::SLOPE_1_R:cell::SLOPE_1_L;
        --p.y;
        cell& c2 = g->getCell(p.x, p.y);
        c2.material = 2;
        c2.col = cell::SQUARE;
        --p.y;
    }
    if(r){
        dirt(g,2, xk, p.x);
        snow(g,lastSnow,xk,p.x);
    }else{
        dirt(g,2, p.x, xk);
        snow(g,lastSnow,p.x,xk);
    }
}
void terrainGen::canyon(grid* g,int w,int d){
    g->ensureRect(right.x, right.y-d, right.x+w, right.y);
    ++right.x;
    for(int i = 0;i< d;++i){
        --right.y;
        ///cell& c2 = g->getCell(right.x, right.y);
        //c2.material = 2;
        //c2.col = cell::SQUARE;
    }
    for(int i = 0;i < w;++i){
        ++right.x;
        cell& c2 = g->getCell(right.x, right.y);
        c2.material = 2;
        c2.col = cell::SQUARE;
    }
    for(int i = 0;i< d;++i){
        ++right.y;
        cell& c2 = g->getCell(right.x, right.y);
        c2.material = 2;
        c2.col = cell::SQUARE;
    }
    dirt(g,2, right.x-w+1, right.x);
    snow(g, lastSnow,right.x-w+1,right.x);
}
void terrainGen::jump(grid* g,vector2 &right, bool r){
    int inc = r?1:-1;
    int xk = right.x+inc;
    for(int i = 0;i < 20;++i){
        right.x += inc;
        g->ensurePoint(right.x, right.y);
        cell& c = g->getCell(right.x, right.y);
        c.material = 2;
        c.col = r?cell::SLOPE_1_R:cell::SLOPE_1_L;
        --right.y;
    }
    right.x+=inc;
    g->ensurePoint(right.x, right.y);
    cell& c = g->getCell(right.x, right.y);
    c.material = 2;
    c.col = r?cell::SLOPE_HALF_TOP_R:cell::SLOPE_HALF_TOP_L;
    right.x+=inc;
    g->ensurePoint(right.x, right.y);
    cell& c2 = g->getCell(right.x, right.y);
    c2.material = 2;
    c2.col = r?cell::SLOPE_HALF_BOTTOM_R:cell::SLOPE_HALF_BOTTOM_L;
    --right.y;
    right.x+=inc;
    g->ensurePoint(right.x, right.y);
    cell& c3 = g->getCell(right.x, right.y);
    c3.material = 2;
    c3.col = cell::SQUARE;
    ++right.y;
    right.x+=inc;
    g->ensurePoint(right.x, right.y);
    cell& c4 = g->getCell(right.x, right.y);
    c4.material = 2;
    c4.col = r?cell::SLOPE_HALF_BOTTOM_L:cell::SLOPE_HALF_BOTTOM_R;
    right.x+=inc;
    g->ensurePoint(right.x, right.y);
    cell& c5 = g->getCell(right.x, right.y);
    c5.material = 2;
    c5.col = r?cell::SLOPE_HALF_TOP_L:cell::SLOPE_HALF_TOP_R;
    right.x+=inc;
    ++right.y;
    g->ensurePoint(right.x, right.y);
    cell& c6 = g->getCell(right.x, right.y);
    c6.material = 2;
    c6.col = r?cell::SLOPE_1_L:cell::SLOPE_1_R;
    right.x+=inc;
    g->ensurePoint(right.x, right.y);
    cell& c7 = g->getCell(right.x, right.y);
    c7.material = 2;
    c7.col = cell::SQUARE;
    if(r){
        dirt(g,2, xk, right.x);
        snow(g,lastSnow,xk,right.x);
    }else{
        dirt(g,2, right.x, xk);
        snow(g,lastSnow,right.x,xk);
    }
}
void terrainGen::generateAt(grid* g,int x,int y){
    while(x >= right.x){
        int k = rand()%6;
        switch (k) {
            case 0:
                jump(g, right,true);
                break;
            case 1:
                steep(g, right, true, rand()%4+4);
                break;
            case 2:
                steepUp(g, rand()%4+4);
                break;
            case 3:
                canyon(g, rand()%9+10, rand()%6+6);
                break;
            case 4:
            case 5:
                flat(g, rand()%17+17);
                break;
        }
    }
    if(g->getCell(x, y).material == 0){
        for(auto p : g->subGrid(x, y, x, INT_MAX)){
            if(p.first.material == 2){
                g->getCell(x, y).material = 2;
                g->getCell(x, y).col = cell::SQUARE;
                return;
            }
        }
        g->getCell(x, y).material = 3;
        g->getCell(x, y).col = cell::NONE;
    }
}