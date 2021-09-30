//
//  gameInstance.cpp
//  StructureSmash
//
//  Created by Milo Brandt on 2/22/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "gameInstance.h"
#include <math.h>

gameInstance::gameInstance(sf::Window& w,grid* g):w(w),offX(0),offY(0),widX(w.getSize().x/15.f),widY(w.getSize().y/15.f),Right(false),Left(false),g(g),mouseType(0){
    plyr.pos = vector2(.5,5);
    srand(time(NULL));
}
int gameInstance::update(){
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
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
            plyr.wantJump = true;
        }
        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up){
            plyr.wantJump = false;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left){
            Left = true;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right){
            Right = true;
        }
        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Left){
            Left = false;
        }
        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Right){
            Right = false;
        }
        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R){
            //Sleep. go back to start?
            plyr.pos = vector2(.5,5);
            plyr.veloc = vector2();
        }
        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::K){
            return 1;
        }
        //window.close();
    }
    plyr.wantMove = (Left?-15:0)+(Right?15:0);
    offX = plyr.pos.x - widX/2;
    offY = plyr.pos.y - widY/2;
    auto vis = g->subGrid(floor(offX)-1, floor(offY)-1, ceil(offX+widX)+1, ceil(offY+widY)+1);
    vis.ensure();
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
                p.first.col = cell::NONE;}
        }
    }
    plyr.move(this, .033);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0, widX, 0, widY, -1, 1);
	glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glLoadIdentity();
    glTranslatef(-offX, -offY,0);
    for(auto p : vis){
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
    glColor3f(0, 0, 0);

    glBegin(GL_QUADS);
    glVertex2f(plyr.pos.x-.1, plyr.pos.y);
    glVertex2f(plyr.pos.x+.1, plyr.pos.y);
    glVertex2f(plyr.pos.x+.1, plyr.pos.y+.66);
    glVertex2f(plyr.pos.x-.1, plyr.pos.y+.66);
    glEnd();
    w.display();
    if(plyr.underx == INT_MAX) return 0;
    if(plyr.pos.y < -1000) return 1;
    if(g->getCell(plyr.underx, plyr.undery).material == 5) return 1;
    if(g->getCell(plyr.underx, plyr.undery).material == 6) return 2;
    return 0;
}