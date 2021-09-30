//
//  grid.cpp
//  StructureSmash
//
//  Created by Milo Brandt on 2/22/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <climits>
#include "grid.h"

grid::grid():zeroX(0){}
cell::cell():material(0),col(NONE){}
cell::cell(cell::collisionBox col,int material):col(col),material(material){}
float cell::linB() const{
    switch (col) {
        case cell::NONE:
            return -1;
        case cell::SQUARE:
            return 1;
        case cell::SLOPE_1_L:
            return 0;
        case cell::SLOPE_1_R:
            return 1;
        case cell::SLOPE_HALF_TOP_L:
            return 0.5;
        case cell::SLOPE_HALF_TOP_R:
            return 1;
        case cell::SLOPE_HALF_BOTTOM_L:
            return 0;
        case cell::SLOPE_HALF_BOTTOM_R:
            return 0.5;
    }
}
float cell::linM() const{
    switch (col) {
        case cell::NONE:
            return 0;
        case cell::SQUARE:
            return 0;
        case cell::SLOPE_1_L:
            return 1;
        case cell::SLOPE_1_R:
            return -1;
        case cell::SLOPE_HALF_TOP_L:
            return 0.5;
        case cell::SLOPE_HALF_TOP_R:
            return -0.5;
        case cell::SLOPE_HALF_BOTTOM_L:
            return 0.5;
        case cell::SLOPE_HALF_BOTTOM_R:
            return -0.5;
    }
}
float cell::getSFriction() const{
    switch(material){
        case 1:
            return 0.4;
        case 2:
            return 1.2;
        case 4:
            return 2;
        default:
            return 1;
    }
}
float cell::getKFriction() const{
    switch(material){
        case 1:
            return 0.1;
        case 2:
            return 1;
        case 4:
            return 1;
        default:
            return 1;
    }
}
cell& grid::getCell(int x,int y){
    std::pair<std::deque<cell>*,int>& k = internal[x+zeroX];
    return k.first->at(y+k.second);
}
grid::iterator::iterator():x(INT_MAX),y(INT_MAX){}
grid::iterator::iterator(grid* g):g(g),x1(INT_MIN),y1(INT_MIN),x2(INT_MAX),y2(INT_MAX),x(-g->zeroX){
    while(x+g->zeroX<g->internal.size() && g->internal[x+g->zeroX].first->empty()) ++x;
    if(x+g->zeroX>=g->internal.size()) *this=iterator();
    else y = -g->internal[x+g->zeroX].second;
}
grid::iterator::iterator(grid* g,int x1,int y1,int x2,int y2):g(g),x1(x1),y1(y1),x2(x2),y2(y2),x(std::max(-g->zeroX,x1)){
    if(x>x2||x+g->zeroX>=g->internal.size()){
        *this=iterator();
        return;
    }
    y = -g->internal[x+g->zeroX].second;
    if(y >= y1 && y <= y2) return;
    ++*this;
}
grid::iterator& grid::iterator::operator++(){
    std::pair<std::deque<cell>*,int>& k = g->internal[x+g->zeroX];
    int minY = -k.second;
    int maxY = k.first->size()-k.second-1;
    if(y<y1||y>y2||y>maxY||y<minY||minY>maxY){
        if(y>=y1){
            ++x;
            if(x+g->zeroX>=g->internal.size()||x>x2){
                *this=iterator();
                return *this;
            }
            y=-g->internal[x+g->zeroX].second;
            if(y>=y1 && y <= y2) return *this; 
            return ++*this;
        }else{
            y=std::max(y1,-k.second);
            return *this;
        }
    }
    ++y;
    if(y>y2||y+k.second>=k.first->size()) return ++*this;
    return *this;
}
bool grid::iterator::operator==(const grid::iterator &b) const{
    return x==b.x && y==b.y;
}
bool grid::iterator::operator!=(const grid::iterator &b) const{
    return x!=b.x || y!=b.y;
}
std::pair<cell&, vector2> grid::iterator::operator*() const{
    return std::make_pair<cell&,vector2>(g->getCell(x, y),vector2(x,y));
}
//WARNING: Strange for loops ahead
void grid::ensurePoint(int x, int y){
    if(x+zeroX<0){
        internal.insert(internal.begin(), -x-zeroX, std::make_pair((std::deque<cell>*)NULL, -y));
        for(int i = -x-zeroX-1;i > 0;--i){
            internal[i].first = new std::deque<cell>();
        }
        internal[0].first = new std::deque<cell>(1,cell());
        zeroX=-x;
        return;
    }
    if(x+zeroX>=internal.size()){
        int i = internal.size();
        internal.insert(internal.end(), x+zeroX-internal.size()+1, std::make_pair((std::deque<cell>*)NULL, -y));
        for(;i < x+zeroX;++i){
            internal[i].first = new std::deque<cell>();
        }
        internal[x+zeroX].first = new std::deque<cell>(1,cell());
        return;
    }
    std::pair<std::deque<cell>*,int>& k = internal[x+zeroX];
    if(y+k.second<0){
        k.first->insert(k.first->begin(), -y-k.second,cell());
        k.second = -y;
        return;
    }
    if(y+k.second>=k.first->size()){
        k.first->insert(k.first->end(), y+k.second-k.first->size()+1,cell());
        return;
    }
}
void grid::ensureRect(int x1,int y1,int x2,int y2){
    for(int i = std::max(x1+zeroX,0);i <= std::min(x2+zeroX,(int)internal.size()-1);i++){
        std::pair<std::deque<cell>*,int>& k = internal[i];
        if(y1+k.second<0){
            k.first->insert(k.first->begin(), -y1-k.second,cell());
            k.second = -y1;
        }
        if(y2+k.second>=k.first->size()){
            k.first->insert(k.first->end(), y2+k.second-k.first->size()+1,cell());
        }
    }
    if(x1+zeroX<0){
        internal.insert(internal.begin(), -x1-zeroX, std::make_pair((std::deque<cell>*)NULL, -y1));
        for(int i = -x1-zeroX-1;i >= 0;--i){
            internal[i].first = new std::deque<cell>(y2-y1+1);
        }
        zeroX=-x1;
    }
    if(x2+zeroX>=internal.size()){
        int i = internal.size();
        internal.insert(internal.end(), x2+zeroX-internal.size()+1, std::make_pair((std::deque<cell>*)NULL, -y1));
        for(;i <= x2+zeroX;++i){
            internal[i].first = new std::deque<cell>(y2-y1+1);
        }
    }
}
grid::iterator grid::beginRect(int x1,int y1,int x2,int y2){
    return grid::iterator(this,x1,y1,x2,y2);
}
grid::iterator grid::begin(){
    return grid::iterator(this);
}
grid::iterator grid::end(){
    return grid::iterator();
}
grid::subgrid::subgrid(grid* g, int x1,int y1,int x2,int y2):g(g),x1(x1),y1(y1),x2(x2),y2(y2){}
grid::iterator grid::subgrid::begin(){
    return g->beginRect(x1, y1, x2, y2);
}
grid::iterator grid::subgrid::end(){
    return g->end();
}
grid::subgrid grid::subGrid(int x1, int y1, int x2, int y2){
    return subgrid(this,x1,y1,x2,y2);
}
void grid::subgrid::ensure(){
    g->ensureRect(x1, y1, x2, y2);
}
int grid::getFirstBlock(int x){
    auto k = internal[x+zeroX];
    int y1 = k.first->size() - k.second - 1;
    while(y1+k.second >= 0){
        if(k.first->at(y1+k.second).col != cell::NONE){
            return y1;
        }
        --y1;
    }
    return INT_MAX;
}
std::pair<vector2, vector2> grid::bounds(){
    int xmin = -zeroX;
    int xmax = -zeroX + internal.size() - 1;
    int ymin = -internal[0].second;
    int ymax = -internal[0].second + internal[0].first->size();
    for(int i = 1;i < internal.size();++i){
        if(internal[i].first->size() == 0) continue;
        if(-internal[i].second < ymin)
            ymin = -internal[i].second;
        if(-internal[i].second+internal[i].first->size() - 1 > ymax)
            ymax = -internal[i].second+internal[i].first->size()-1;
    }
    return std::make_pair(vector2(xmin,ymin), vector2(xmax,ymax));
}
grid::~grid(){
    for(auto p : internal) delete p.first;
}