//
//  levelBuild.cpp
//  StructureSmash
//
//  Created by Milo Brandt on 2/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "levelBuild.h"
#include "grid.h"
#include "gameInstance.h"
#include <iostream>
#include <SFML/Window.hpp>

structure::~structure(){}
void levelBuild::generate(){
    if(g != 0) delete g;
    g = new grid();
    trace.clear();
    g->ensureRect(0,-5, 1, 5);
    for(auto p : *g){
        if(p.second.y < -2){
            p.first.material = 2; //Dirt.
            p.first.col = cell::SQUARE;
        }else{
            p.first.material = 3; //Air
            p.first.col = cell::NONE;
        }
    }
    vector2 r(1,-3);
    trace.push_back(vector2(0,5));
    trace.push_back(vector2(1,-3));
    for(structure* s : stuff){
        r = s->build(g, r);
        trace.push_back(r);
    }
    for(int x = 0;x<=r.x;++x){
        int y = g->getFirstBlock(x);
        if(y == INT_MAX) continue;
        if(g->getCell(x, y).material == 5) continue;
        g->ensurePoint(x, y+2);
        int mat = 4;
        if( g->getCell(x, y).material == 6)mat = 6;

        for(int i = 1;i < 2;++i){
            cell& c = g->getCell(x,y+i);
            c.material = mat;
            c.col = cell::SQUARE;
        }
        cell& c2 = g->getCell(x,y+2);
        c2.material = mat;
        c2.col = g->getCell(x, y).col;
    }
    for(int x = 0;x<=r.x;++x){
        int y = g->getFirstBlock(x);
        if(y == INT_MAX) continue;
        if(g->getCell(x, y).material == 5) continue;
        int mat = 1;
        if( g->getCell(x, y).material == 6)mat = 6;
        g->ensurePoint(x, y+4);
        for(int i = 1;i < 4;++i){
            cell& c = g->getCell(x,y+i);
            c.material = mat;
            c.col = cell::SQUARE;
        }
        cell& c2 = g->getCell(x,y+4);
        c2.material = mat;
        c2.col = g->getCell(x, y).col;
    }
    bounds = g->bounds();
    auto vis = g->subGrid(bounds.first.x, bounds.first.y, bounds.second.x, bounds.second.y);
    vis.ensure();
    for(auto p : vis){
        if(p.first.material == 0){
            for(auto p2 : g->subGrid(p.second.x, p.second.y, p.second.x, INT_MAX)){
                if(p2.first.material == 2 || p2.first.material == 6){
                    p.first.material = 2;
                    p.first.col = cell::SQUARE;
                }
                if(p2.first.material == 5){
                    p.first.material = 5;
                    p.first.col = cell::SQUARE;
                }
                
            }
            if(p.first.material == 0){
                p.first.material = 3;
                p.first.col = cell::NONE;
            }
        }
    }

    float aspect = 1.333333;
    vector2 dif = (bounds.second-bounds.first);
    if(dif.x > aspect*dif.y){
        bounds.second.y += (dif.x - aspect*dif.y)/aspect;
    }
    if(dif.x < aspect*dif.y){
        bounds.second.x += (aspect*dif.y - dif.x);
    }
}
namespace{
    class jump : public structure{
    public:
        int d;
        jump(int d):d(d){}
        vector2 build(grid* g,vector2 s){
            auto vis = g->subGrid(s.x, s.y-d-2, s.x+d+7, s.y);
            vis.ensure();
            bool r = true;
            vector2 right = s;
            int inc = r?1:-1;
            int xk = right.x+inc;
            for(int i = 0;i < d;++i){
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
            /*if(r){
                dirt(g,2, xk, right.x);
                snow(g,lastSnow,xk,right.x);
            }else{
                dirt(g,2, right.x, xk);
                snow(g,lastSnow,right.x,xk);
            }*/
            for(auto p : vis){
                if(p.first.material == 0){
                    for(auto p2 : g->subGrid(p.second.x, p.second.y, p.second.x, INT_MAX)){
                        if(p2.first.material == 2){
                            p.first.material = 2;
                            p.first.col = cell::SQUARE;
                        }
                        if(p2.first.material == 5){
                            p.first.material = 5;
                            p.first.col = cell::SQUARE;
                        }

                    }
                    if(p.first.material == 0){
                    p.first.material = 3;
                    p.first.col = cell::NONE;
                    }
                }
            }
            return right;
        }
    };
    class easyWin : public structure{
    public:
        int w;
        easyWin(int w):w(w){}
        vector2 build(grid* g,vector2 s){
            g->ensureRect(s.x+1, s.y, s.x+w+1, s.y);
            for(int i = 0;i < w;++i){
                ++s.x;
                g->getCell(s.x, s.y).material = (i==0||i==w-1)?2:6;
                g->getCell(s.x, s.y).col = cell::SQUARE;
            }
            return s;
        }
    };
    class backBoard : public structure{
    public:
        int d;
        backBoard(int d):d(d){}
        vector2 build(grid* g,vector2 s){
            g->ensureRect(s.x+1, s.y, s.x+1, s.y+d);
            ++s.x;
            vector2 o = s;
            for(int i = 0;i < d;++i){
                ++s.y;
                g->getCell(s.x, s.y).material = 2;
                g->getCell(s.x, s.y).col = cell::SQUARE;
            }
            return o;
        }
    };
    class canyon : public structure{
    public:
        int d;
        int w;
        canyon(int d,int w):d(d),w(w){}
        vector2 build(grid* g,vector2 s){
            vector2 right = s;
            g->ensureRect(right.x, right.y-d, right.x+w, right.y);
            for(int i = 0;i< d;++i){
                --right.y;
               // cell& c2 = g->getCell(right.x, right.y);
                //c2.material = 2;
               // c2.col = cell::SQUARE;
            }
            for(int i = 0;i < w;++i){
                ++right.x;
                cell& c2 = g->getCell(right.x, right.y);
                c2.material = i==w-1?2:5;
                c2.col = cell::SQUARE;
            }
            for(int i = 0;i< d;++i){
                ++right.y;
                cell& c2 = g->getCell(right.x, right.y);
                c2.material = 2;
                c2.col = cell::SQUARE;
            }
            return right;
        }
    };
    class slope : public structure{
    public:
        int l;
        slope(int l):l(l){}
        vector2 build(grid* g,vector2 s){
            g->ensureRect(s.x+1, s.y-2*l, s.x+l, s.y);
            for(int i = 0;i<l;++i){
                ++s.x;
                g->getCell(s.x, s.y).material = 2;
                g->getCell(s.x, s.y).col = cell::SLOPE_1_R;
                --s.y;
                g->getCell(s.x, s.y).material = 2;
                g->getCell(s.x, s.y).col = cell::SQUARE;
                --s.y;
            }
            ++s.y;
            return s;
        }
    };
    class slopeUp : public structure{
    public:
        int l;
        slopeUp(int l):l(l){}
        vector2 build(grid* g,vector2 s){
            g->ensureRect(s.x+1, s.y, s.x+l, s.y+2*l);
            --s.y;
            for(int i = 0;i<l;++i){
                ++s.x;
                ++s.y;
                g->getCell(s.x, s.y).material = 2;
                g->getCell(s.x, s.y).col = cell::SQUARE;
                ++s.y;
                g->getCell(s.x, s.y).material = 2;
                g->getCell(s.x, s.y).col = cell::SLOPE_1_L;
            }
            return s;
        }
    };
}
structure* structure::makeJump(int d){
    return new jump(d);
}
structure* structure::makeCanyon(int w,int d){
    return new canyon(w,d);
}
structure* structure::makeSlope(int d){
    return new slope(d);
}
structure* structure::makeSlopeUp(int d){
    return new slopeUp(d);
}
structure* structure::makeWinZone(int w){
    return new easyWin(w);
}
structure* structure::makeBack(int h){
    return new backBoard(h);
}
levelBuild::levelBuild(sf::Window& w,std::vector<structure*> s):g(0),w(w),stuff(s){
    generate();
    dragging = -1;
    anchored = 1;
    hover = -1;
}
bool levelBuild::update(){
    sf::Event event;
    while (w.pollEvent(event))
    {
        // Close window : exit
        if (event.type == sf::Event::Closed)
            w.close();
        // Escape pressed : exit
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
            w.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){
            return true;
        }
        if(event.type == sf::Event::MouseButtonPressed){
            float xC = event.mouseButton.x*(bounds.second.x - bounds.first.x)/w.getSize().x + bounds.first.x+.5;
            /*float yC = (w.getSize().y - event.mouseButton.y)/(bounds.second.y - bounds.first.y + 1) + bounds.first.x;*/
            for(int i = 2;i < trace.size()-anchored;++i){
                if(trace[i].x > xC){
                    dragging = i - 2;
                    dragO = trace[i-1].x - xC;
                    break;
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased && dragging != -1){
            dragging = -1;
        }
        //window.close();
    }
    //float aspect = 1.33333; //x/y = 4/3
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(bounds.first.x, bounds.second.x+1, bounds.first.y, bounds.second.y+1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    for(auto p : *g){
        if(p.first.material == 3 || p.first.material == 0) continue;
        g->ensurePoint(p.second.x, p.second.y+1);
        if(p.first.material != 3 && (g->getCell(p.second.x, p.second.y+1).material == 1 || g->getCell(p.second.x, p.second.y+1).material == 4)){
            
            if(g->getCell(p.second.x, p.second.y+1).material == 1){
                glColor3f(1, 1, 1);
            }else{
                glColor3f(0.6, 0.4, 0.2);
            }
            glBegin(GL_QUADS);
            glVertex2f(p.second.x, p.second.y);
            glVertex2f(p.second.x+1, p.second.y);
            glVertex2f(p.second.x+1, 1+p.second.y);
            glVertex2f(p.second.x, 1+p.second.y);
            
            glEnd();
        }
        if(p.first.material == 1){
            glColor3f(1, 1, 1);
        }else if(p.first.material == 4){
            glColor3f(.6, .4, .2);
        }else if(p.first.material == 2){
            glColor3f(.6,.6,.6);
        }else if(p.first.material == 5){
            glColor3f(1, 0, 0);
        }else if(p.first.material == 6){
            glColor3f(0, 1, 0);
        }
        
        if(p.first.col == cell::SQUARE){
            glBegin(GL_QUADS);
            glVertex2f(p.second.x, p.second.y);
            glVertex2f(p.second.x+1, p.second.y);
            glVertex2f(p.second.x+1, 1+p.second.y);
            glVertex2f(p.second.x, 1+p.second.y);
            
            glEnd();
        }else if(p.first.col == cell::SLOPE_1_L){
            glBegin(GL_TRIANGLES);
            glVertex2f(p.second.x, p.second.y);
            glVertex2f(p.second.x+1, 1+p.second.y);
            glVertex2f(p.second.x+1, p.second.y);
            
            glEnd();
        }else if(p.first.col == cell::SLOPE_1_R){
            glBegin(GL_TRIANGLES);
            glVertex2f(p.second.x, p.second.y);
            glVertex2f(p.second.x+1, p.second.y);
            glVertex2f(p.second.x, 1+p.second.y);
            
            glEnd();
        }else if(p.first.col == cell::SLOPE_HALF_TOP_L){
            glBegin(GL_QUADS);
            glVertex2f(p.second.x, .5+p.second.y);
            glVertex2f(p.second.x+1, 1+p.second.y);
            glVertex2f(p.second.x+1, p.second.y);
            glVertex2f(p.second.x, p.second.y);
            
            glEnd();
        }else if(p.first.col == cell::SLOPE_HALF_TOP_R){
            glBegin(GL_QUADS);
            glVertex2f(p.second.x+1, .5+p.second.y);
            glVertex2f(p.second.x, 1+p.second.y);
            glVertex2f(p.second.x, p.second.y);
            glVertex2f(p.second.x+1, p.second.y);
            glEnd();
        }else if(p.first.col == cell::SLOPE_HALF_BOTTOM_L){
            glBegin(GL_TRIANGLES);
            glVertex2f(p.second.x, p.second.y);
            glVertex2f(p.second.x+1, .5+p.second.y);
            glVertex2f(p.second.x+1, p.second.y);
            
            glEnd();
        }else if(p.first.col == cell::SLOPE_HALF_BOTTOM_R){
            glBegin(GL_TRIANGLES);
            glVertex2f(p.second.x, p.second.y);
            glVertex2f(p.second.x+1, p.second.y);
            glVertex2f(p.second.x, .5+p.second.y);
            
            glEnd();
        }
    }
    hover = -1;
    float xC = sf::Mouse::getPosition(w).x*(bounds.second.x - bounds.first.x)/w.getSize().x + bounds.first.x+.5;
    for(int i = 2;i < trace.size()-anchored;++i){
        if(trace[i].x > xC){
            hover = i - 2;
            break;
        }
    }
    if(dragging == -1 && hover > -1){
        glColor4f(1, 1, 0, 0.25);
        glBegin(GL_QUADS);
        glVertex2f(trace[hover+1].x,-10000);
        glVertex2f(trace[hover+1].x,10000);
        glVertex2f(trace[hover+2].x+1,10000);
        glVertex2f(trace[hover+2].x+1,-10000);

        glEnd();
    }
    if(dragging != -1){
        float xC = sf::Mouse::getPosition(w).x*(bounds.second.x - bounds.first.x)/w.getSize().x + bounds.first.x+.5;
        /*float yC = (w.getSize().y - event.mouseButton.y)/(bounds.second.y - bounds.first.y + 1) + bounds.first.x;*/
        bool changed = false;
        float desired = xC + dragO;

        {
            float noChange = abs(desired-trace[dragging + 1].x);
            float less = dragging==0?100000000:abs(desired-trace[dragging].x);
            float more = dragging>=stuff.size()-anchored-1?100000000:abs(desired-(trace[dragging+1].x+trace[dragging+3].x-trace[dragging+2].x));
            if(noChange <= less+.1 && noChange <= more+.1){}else{
            changed = true;
            if(less < more){
                std::swap(stuff[dragging], stuff[dragging-1]);
                --dragging;
            }else{
                std::swap(stuff[dragging], stuff[dragging+1]);
                ++dragging;
            }}
        }
        if(changed) generate();
        glColor4f(1, 1, 0, 0.25);
        glBegin(GL_QUADS);
        glVertex2f(trace[dragging+1].x,-10000);
        glVertex2f(trace[dragging+1].x,10000);
        glVertex2f(trace[dragging+2].x+1,10000);
        glVertex2f(trace[dragging+2].x+1,-10000);
        
        glEnd();
    }
    w.display();
    return false;
}
gameInstance* levelBuild::game(){
    return new gameInstance(w, g);
}
levelBuild::~levelBuild(){
    delete g;
    for(structure* s : stuff) delete s;
}