//
//  main.cpp
//  Mississippi10
//
//  Created by Milo Brandt on 8/23/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <string>
#include "gameState.h"
#include "enemy.h"
#include "ResourcePath.hpp"
enemy* flying(float xc,float yc,float rad,float radStep,float rad0,vector2 size){
    enemy* e = new enemy(rectangle(), 300);
    for(int i = 0;i<300;++i){
        float ang = rad0 + radStep*i;
        vector2 pos = vector2(xc+rad*sin(ang),yc+rad*cos(ang));
        e->positions[i] = rectangle(pos,pos+size);
    }
    return e;
}
void makeMovingPlat(rectangle start,vector2 veloc,gameState& g){
    rectangle* r = new rectangle[g.frames];
    g.movingRect.push_back(r);
    for(int i = 0;i<300;++i){
        r[i] = rectangle(start.upLeft+veloc*i,start.downRight+veloc*i);
    }
}
class button : public enemy{
public:
    int platNum;
    rectangle r;
    vector2 veloc;
    button(rectangle pos,rectangle r,vector2 v,gameState& g):enemy(pos,300,false),platNum(platNum),r(r),veloc(v){
        platNum = g.movingRect.size();
        makeMovingPlat(r, vector2(), g);
    }
    void onDeath(int frame,gameState* g){
        for(int i = killFrame;i < frame && i < g->frames;++i){
            g->movingRect[platNum][i] = r;
        }
        for(int i = frame;i < g->frames;++i){
            g->movingRect[platNum][i] = rectangle(r.upLeft + (i-frame)*veloc,r.downRight + (i-frame)*veloc);
        }
    }
};
int main(int argc, const char * argv[])
{
    sf::RenderWindow main(sf::VideoMode(640,480),"Codename Mississippi-10");
    sf::Event event;
    main.setFramerateLimit(30);
    gameState game(10, 10, 300, 1.0/30, .25, vector2(0,3));
    float gridSize = 40;
    //sf::CircleShape ninjaC(game.nRad*gridSize);
    //ninjaC.setFillColor(sf::Color::Black);
    sf::Texture* ninjaRun = new sf::Texture[4];
    if (!ninjaRun[0].loadFromFile(resourcePath() + "/ninjaRun1.png"))
    {
        std::cout << "Done";
    }
    if (!ninjaRun[1].loadFromFile(resourcePath() + "/ninjaRun2.png"))
    {
        std::cout << "Done";
    }
    if (!ninjaRun[2].loadFromFile(resourcePath() + "/ninjaRun3.png"))
    {
        std::cout << "Done";
    }
    ninjaRun[3].loadFromFile(resourcePath() + "/ninjaRun4.png");
    sf::Texture* dust = new sf::Texture[5];
    dust[0].loadFromFile(resourcePath() + "dust1.png");
    dust[1].loadFromFile(resourcePath() + "dust2.png");
    dust[2].loadFromFile(resourcePath() + "dust3.png");
    dust[3].loadFromFile(resourcePath() + "dust4.png");
    dust[4].loadFromFile(resourcePath() + "dust5.png");
    sf::Texture spikes;
    spikes.loadFromFile(resourcePath() + "/spikes.png");
    spikes.setRepeated(true);
    //spikes.setRepeated(true);
    sf::Texture targT;
    targT.loadFromFile(resourcePath() + "Enemy.png");
    sf::Sprite targ(targT);
    sf::Sprite spikesC(spikes);
    sf::Sprite ninjaC(ninjaRun[0]);
    sf::Sprite dustC(dust[0]);
    dustC.setColor(sf::Color(255,255,255,128));
    sf::RectangleShape plat(sf::Vector2f(gridSize,gridSize));
    plat.setFillColor(sf::Color::Black);
    sf::RectangleShape bottomPanel(sf::Vector2f(640,80));
    bottomPanel.setPosition(0, 400);
    bottomPanel.setFillColor(sf::Color(128,128,128));
    sf::RectangleShape timeline(sf::Vector2f(300,60));
    timeline.setPosition(20, 410);
    timeline.setFillColor(sf::Color::Black);
    sf::RectangleShape targB(sf::Vector2f(10,10));
    targB.setFillColor(sf::Color::Red);
    sf::Text victory;
    sf::Clock blinkClock;
    victory.setPosition(260,20);
    victory.setString("VICTORY!");
    victory.setColor(sf::Color::White);
    sf::Text replay;
    replay.setString("Instant Replay");
    replay.setColor(sf::Color::White);
    replay.setScale(.7, .7);
    replay.setPosition(260,55);
    sf::Text t;
    t.setPosition(340, 405);
    t.setString("Click to select a ninja. Arrow keys to move.\nSpace bar to start time, r to reverse\nThe timeline may be dragged\nGet the ninja off the screen in time.");
    t.setColor(sf::Color::White);
    //t.setScale(.5,.5);
    t.setCharacterSize(15);
    sf::RectangleShape timeIndicator(sf::Vector2f(2,60));
    timeIndicator.setFillColor(sf::Color::Yellow);
    timeIndicator.setOutlineColor(sf::Color::Yellow);
    game.staticRect.push_back(rectangle(vector2(0,9), vector2(10,10)));

    game.agents.push_back(ninja(vector2(4.5,1),vector2(),1200));
    game.agents.push_back(ninja(vector2(5.5,1),vector2(),1200));
    

    game.selected = &game.agents.front();
    bool wPressed = false;
    bool aPressed = false;
    bool timelineDrag = false;
    bool dPressed = false;
    bool firing = false;
    float speed = 1;
    bool winlap = false;
    float ospeed = 0;
    float speedac = 0;
    bool genLevel = true;
    int levelN = 0;
    sf::VertexArray lines(sf::LinesStrip, 2);
    while(main.isOpen()){
        if(game.win){
            if(!winlap){
            winlap = true;
            speed = -3;
            game.selected = 0;
                game.win = false;
            }else{
                winlap = false;
                ++levelN;
                genLevel = true;
            }
        }
        if(genLevel){
            genLevel = false;
            if(levelN == 0){
                game = gameState(16, 10, 300, .033333, .25, vector2(0,6));
                game.agents.push_back(ninja(vector2(8,3),vector2(),300));
                game.staticRect.push_back(rectangle(vector2(0,8), vector2(16,10)));
                t.setString("Click to select a ninja. Arrow keys to move.\nSpace bar to start time, r to reverse\nThe timeline may be dragged\nGet the ninja off the screen in time.");
                speed = 0;
                ospeed = 1;
            }else if(levelN == 1){
                game = gameState(16, 10, 300, .033333, .25, vector2(0,6));
                game.agents.push_back(ninja(vector2(7,3),vector2(),300));
                game.agents.push_back(ninja(vector2(9,3),vector2(),300));
                game.agents.push_back(ninja(vector2(8,3),vector2(),300));
                game.agents.push_back(ninja(vector2(6,3),vector2(),300));
                game.agents.push_back(ninja(vector2(10,3),vector2(),300));
                game.agents.push_back(ninja(vector2(11,3),vector2(),300));
                game.agents.push_back(ninja(vector2(5,3),vector2(),300));

                game.staticRect.push_back(rectangle(vector2(0,8), vector2(16,10)));
                t.setString("Sometimes there will be many ninjas.\nEach saves their actions.\nUse the timeline and pausing (space bar) to\ngive simultaneous orders.");
                speed = 0;
                ospeed = 1;
            }else if(levelN == 2){
                game = gameState(16, 10, 300, .033333, .25, vector2(0,6));
                game.agents.push_back(ninja(vector2(7,3),vector2(),300));
                game.staticRect.push_back(rectangle(vector2(0,8), vector2(16,10)));
                game.enemies.push_back(new enemy(rectangle(vector2(10.5,7.5),vector2(11,8)), 300));
                t.setString("In being so awesome, ninjas make enemies.\nIt's okay though, because each ninja carries\na LASER SHURIKEN! (just one though)\nThe shurikens have a 'k' button on them...");
                speed = 0;
                ospeed = 1;
            }else if(levelN == 3){
                game = gameState(16, 10, 300, .033333, .25, vector2(0,6));
                game.agents.push_back(ninja(vector2(7,3),vector2(),300));
                game.staticRect.push_back(rectangle(vector2(0,8), vector2(16,10)));
                game.enemies.push_back(flying(8, 30, -25, -0.01, M_PI/8, vector2(.5,.5)));
                game.enemies.push_back(flying(8, 30, -26, 0.01, -M_PI/8, vector2(.5,.5)));

                t.setString("Ninjas have a saying about how two birds\ncan be killed by just a single laser-shuriken.\n(But they're ninjas, so they don't say it aloud)\nUse 'e' to slow time, 't' to speed it up.");
                speed = 0;
                ospeed = 1;
            }else if(levelN == 4){
                game = gameState(16, 10, 300, .033333, .25, vector2(0,6));
                game.agents.push_back(ninja(vector2(7,7.5),vector2(),300));
                game.agents.push_back(ninja(vector2(1,7.5),vector2(),300));

                game.staticRect.push_back(rectangle(vector2(0,8), vector2(16,10)));
                game.enemies.push_back(flying(8, 30, -26, -0.01, M_PI/8, vector2(.5,.5)));
                game.enemies.push_back(flying(8, 30, -27, 0.004, -M_PI/8, vector2(.5,.5)));
                game.enemies.push_back(flying(8, 50, -49, 0.0025, -M_PI/16, vector2(.5,.5)));
                makeMovingPlat(rectangle(vector2(-100,6),vector2(12,7)),vector2(0,0), game);
                makeMovingPlat(rectangle(vector2(14.5,0),vector2(16,16)),vector2(0,0), game);
                makeMovingPlat(rectangle(vector2(10,2),vector2(11,8)),vector2(0,.1), game);
                t.setString("Sometimes the two-bird theory is hard to\napply. (But the ninjas don't care)");
                speed = 0;
                ospeed = 1;
            }else if(levelN == 5){
                game = gameState(16, 10, 300, .033333, .25, vector2(0,6));
                game.agents.push_back(ninja(vector2(1.25,7.5),vector2(),300));
                game.agents.push_back(ninja(vector2(1.75,7.5),vector2(),300));
                
                game.staticRect.push_back(rectangle(vector2(0,8), vector2(2,10)));
                game.staticRect.push_back(rectangle(vector2(0,0), vector2(1,10)));
                game.staticRect.push_back(rectangle(vector2(14,8), vector2(16,10)));
                game.staticRect.push_back(rectangle(vector2(6,0), vector2(7,7)));
                game.spikes.push_back(rectangle(vector2(2,9), vector2(14,10)));

                game.enemies.push_back(new button(rectangle(vector2(1,1),vector2(1.2,1.5)), rectangle(vector2(2,8),vector2(3,9)), vector2(.1,0), game));
                t.setString("Ninjas can't operate buttons*.\nBut they can throw shurikens at them\n(Remeber 'e' and 't' slow/speed time)\n*This, like all facts in the game, is true.");
                speed = 0;
                ospeed = 1;
            }else if(levelN == 6){
                game = gameState(16, 10, 300, .033333, .25, vector2(0,6));
                game.agents.push_back(ninja(vector2(1.25,7.5),vector2(),300));
                game.agents.push_back(ninja(vector2(1.75,7.5),vector2(),300));
                
                game.staticRect.push_back(rectangle(vector2(0,8), vector2(2,10)));
                game.staticRect.push_back(rectangle(vector2(0,0), vector2(1,10)));
                game.staticRect.push_back(rectangle(vector2(14,8), vector2(16,10)));
                game.staticRect.push_back(rectangle(vector2(6,0), vector2(7,7)));
                game.spikes.push_back(rectangle(vector2(2,9), vector2(14,10)));
                
                game.enemies.push_back(new button(rectangle(vector2(1,1),vector2(1.2,1.5)), rectangle(vector2(2,8),vector2(3,9)), vector2(.1,0), game));
                t.setString("Birds also can't operate buttons.");
                game.enemies.push_back(flying(8, 30, -26, -0.0025, M_PI/8, vector2(.5,.5)));
                game.enemies.push_back(flying(8, 30, -27, 0.002, -M_PI/8, vector2(.5,.5)));
                game.enemies.push_back(flying(8, 50, -49, 0.00125, -M_PI/16, vector2(.5,.5)));
                speed = 0;
                ospeed = 1;
            }else if(levelN == 7){
                game = gameState(16, 10, 300, .033333, .25, vector2(0,6));
                game.agents.push_back(ninja(vector2(1.5,7.5),vector2(),300));
                game.agents.push_back(ninja(vector2(1.5,5.5),vector2(),300));
                game.agents.push_back(ninja(vector2(1.5,3.5),vector2(),300));
                game.agents.push_back(ninja(vector2(1.5,1.5),vector2(),300));

                game.staticRect.push_back(rectangle(vector2(0,8), vector2(16,10)));
                game.staticRect.push_back(rectangle(vector2(0,6), vector2(16,7)));
                game.staticRect.push_back(rectangle(vector2(0,4), vector2(16,5)));
                game.staticRect.push_back(rectangle(vector2(0,2), vector2(16,3)));
                game.staticRect.push_back(rectangle(vector2(0,0), vector2(16,1)));
                game.staticRect.push_back(rectangle(vector2(0,0), vector2(1,10)));
                game.enemies.push_back(new button(rectangle(vector2(5,7),vector2(5.5,7.2)), rectangle(vector2(3,4),vector2(4,7)), vector2(0,-.02), game));
                game.enemies.push_back(new button(rectangle(vector2(1,5.25),vector2(1.2,5.75)), rectangle(vector2(9,4),vector2(10,5.1)), vector2(0,.02), game));
                game.enemies.push_back(new button(rectangle(vector2(1,5.25),vector2(1.2,5.75)), rectangle(vector2(9,4),vector2(10,5.1)), vector2(0,.0125), game));
                game.enemies.push_back(new button(rectangle(vector2(1,5.25),vector2(1.2,5.75)), rectangle(vector2(9,4),vector2(10,5.1)), vector2(0,.015), game));
                game.enemies.push_back(new button(rectangle(vector2(1,5.25),vector2(1.2,5.75)), rectangle(vector2(9,4),vector2(10,5.1)), vector2(0,.01), game));
                game.enemies.push_back(new button(rectangle(vector2(1,5.25),vector2(1.2,5.75)), rectangle(vector2(9,4),vector2(10,5.1)), vector2(0,.005), game));
                game.enemies.push_back(new button(rectangle(vector2(1,5.25),vector2(1.2,5.75)), rectangle(vector2(9,4),vector2(10,5.1)), vector2(0,.0025), game));
                game.enemies.push_back(new button(rectangle(vector2(1,5.25),vector2(1.2,5.75)), rectangle(vector2(8,1),vector2(9,6)), vector2(0,-.2), game));

                t.setString("Some building have strange designs.\nOr maybe some level designers are lazy");
                speed = 0;
                ospeed = 1;
            }else if(levelN == 8){
                game = gameState(16, 10, 300, .033333, .25, vector2(0,6));
                game.agents.push_back(ninja(vector2(8,3),vector2(),300));
                game.staticRect.push_back(rectangle(vector2(0,8), vector2(16,10)));
                game.staticRect.push_back(rectangle(vector2(0,0), vector2(2,10)));
                game.staticRect.push_back(rectangle(vector2(0,0), vector2(16,2)));
                game.staticRect.push_back(rectangle(vector2(14,0), vector2(16,10)));

                t.setString("You win! As a prize enjoy a:\nCOMPLEMENTARY NINJA-IN-A-BOX!\nAs a bonus, use the ninja for 10.5 seconds\nThat's 5% extra! WOW!");
                speed = 0;
                ospeed = 1;
            }
        }
        while (main.pollEvent(event))
        {
        // check the type of the event...
            switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    main.close();
                    break;
                
                // key pressed
                case sf::Event::KeyPressed:
                    if(game.w != 0 && !winlap){
                        if(event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) wPressed = true;
                        if(event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) aPressed = true;
                        if(event.key.code == 'D' || event.key.code == sf::Keyboard::Right) dPressed = true;
                        if(event.key.code == sf::Keyboard::Space){
                            if(speed == 0){
                                speed = ospeed;
                            }else{
                                ospeed = speed;
                                speed = 0;
                            }
                        }
                        if (event.key.code == sf::Keyboard::F1)
                        {
                            sf::Image Screen = main.capture();
                            Screen.saveToFile("screenshot.png");
                        }
                        if(event.key.code == sf::Keyboard::R){
                            if(speed == 0){
                                ospeed = -ospeed;
                            }else{
                                speed = -speed;
                            }
                        }
                        if(event.key.code == sf::Keyboard::E){
                            if(speed == 0){
                                ospeed = ospeed/2;
                            }else{
                                speed = speed/2;
                            }
                        }
                        if(event.key.code == sf::Keyboard::T){
                            if(speed == 0){
                                ospeed = ospeed*2;
                            }else{
                                speed = speed*2;
                            }
                        }
                        if(event.key.code == sf::Keyboard::K){
                            if(firing){
                                firing = false;
                                //speed = ospeed;
                            }else if(game.selected != 0){
                                firing = true;
                                /*if(speed != 0)
                                    ospeed = speed;
                                speed = 0;*/
                            }
                        }
                    }
                    break;
                case sf::Event::KeyReleased:
                    if(game.w != 0){
                        if(event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) wPressed = false;
                        if(event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) aPressed = false;
                        if(event.key.code == 'D' || event.key.code == sf::Keyboard::Right) dPressed = false;
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                
                    if(game.w != 0 && !winlap){
                        if(firing && game.selected != 0){
                            if(game.selected->shurikenTime != -1 && game.selected->shurikenTime < game.frame){
                                int now = game.frame;
                                game.frame = game.selected->shurikenTime;
                                for(enemy* e : game.enemies){
                                    if(e->killFrame >= game.frame){
                                        e->onDeath(game.frames+1, &game);
                                        e->killFrame = game.frames+1;
                                    }
                                }
                                while(game.frame < now) game.update();
                            }
                            game.selected->shurikenTime = game.frame;
                            game.selected->shurikenTarg = vector2(event.mouseButton.x,event.mouseButton.y)/gridSize;
                            if(game.frame == 0){
                            game.fireShuriken(game.selected->pos[game.frame], game.selected->shurikenTarg);
                            }else{
                                game.frame--;
                                game.update();
                            }
                            //speed = ospeed;
                            firing = false;
                        }else{
                    float x = event.mouseButton.x;
                    float y = event.mouseButton.y;
                    if(x > 20 && x < 320 && y > 410 && y < 470){
                        timelineDrag = true;
                        int t = (int)x-20;
                        if(t >= game.frames) t = game.frames - 1;
                        if(t < 0) t = 0;

                        if(t < game.frame){
                            game.frame = t;
                        }else{
                            while(game.frame < t){
                                game.update();
                            }
                        }
                    }else if(y < 400){
                        vector2 p(x,y);
                        p/=gridSize;
                        game.selected = &game.agents.front();
                        for(ninja& n : game.agents){
                            if((p-n.pos[game.frame]).magnitudeSq() < (p-game.selected->pos[game.frame]).magnitudeSq()){
                                game.selected = &n;
                            }
                        }
                    }
                        }
                }
                    break;
                case sf::Event::MouseButtonReleased:
                    timelineDrag = false;
                    break;
                    
                case sf::Event::MouseMoved:
                {
                    float x = event.mouseMove.x;
                    float y = event.mouseMove.y;
                    if(timelineDrag){
                        timelineDrag = true;
                        int t = (int)x-20;
                        if(t >= game.frames) t = game.frames - 1;
                        if(t < 0) t = 0;
                        if(t < game.frame){
                            game.frame = t;
                        }else{
                            while(game.frame < t){
                                game.update();
                            }
                        }
                    }
                }
                    break;
                // we don't process other types of events
                default:
                    break;
            }
        }
        main.clear(sf::Color::Blue);
        if(game.w != 0){
            if(game.selected != 0){
                if(wPressed && game.canJump(*game.selected))
                    game.selected->movAction[game.frame+1] = 'W';
                else{
                    if(aPressed && !dPressed){
                        game.selected->movAction[game.frame+1] = 'A';
                    }else if(!aPressed && dPressed){
                        game.selected->movAction[game.frame+1] = 'D';
                    }else{
                        game.selected->movAction[game.frame+1] = '.';
                    }
                }
            }
            for(enemy* const& e : game.enemies){
                if(e->killFrame < game.frame) continue;
                rectangle r = e->positions[game.frame];
                if(e->critical){
                targ.setPosition(gridSize*r.upLeft.x, gridSize*r.upLeft.y);
                main.draw(targ);
                }else{
                    targB.setSize(gridSize*sf::Vector2f(r.downRight.x-r.upLeft.x,r.downRight.y-r.upLeft.y));
                    targB.setPosition(gridSize*r.upLeft.x, gridSize*r.upLeft.y);
                    main.draw(targB);

                }
            }
            for(rectangle const& r : game.staticRect){
                plat.setSize(gridSize*sf::Vector2f(r.downRight.x-r.upLeft.x,r.downRight.y-r.upLeft.y));
                plat.setPosition(gridSize*r.upLeft.x, gridSize*r.upLeft.y);
                main.draw(plat);
            }
            for(rectangle* const& rp : game.movingRect){
                rectangle r = rp[game.frame];
                plat.setSize(gridSize*sf::Vector2f(r.downRight.x-r.upLeft.x,r.downRight.y-r.upLeft.y));
                plat.setPosition(gridSize*r.upLeft.x, gridSize*r.upLeft.y);
                main.draw(plat);
            }
            for(rectangle const& r : game.spikes){
                //spikesC.setScale(sf::Vector2f(r.downRight.x-r.upLeft.x,r.downRight.y-r.upLeft.y));
                spikesC.setTextureRect(sf::IntRect(0, 0, gridSize*(r.downRight.x-r.upLeft.x), gridSize*(r.downRight.y-r.upLeft.y)));
                spikesC.setPosition(gridSize*r.upLeft.x, gridSize*r.upLeft.y);
                main.draw(spikesC);
            }
            if(firing && game.selected == 0) firing = false;
            int off = 0;
            for(ninja& n : game.agents){
                off += 3;
                int pt = ((off+game.frame)/2)%4;
                if(pt==3) pt = 1;
                if(!game.canJump(n)) pt = 3;
                if(fabs(n.veloc[game.frame].x) < .1){
                    pt = 0;
                }
                int dpt = ((off+game.frame))%5;
                ninjaC.setTexture(ninjaRun[pt]);
                if(&n == game.selected){
                    ninjaC.setColor(sf::Color(255,255,255));
                }else{
                    ninjaC.setColor(sf::Color(128,128,128));
                }
                switch(n.movAction[game.frame]){
                    case 'A':
                        ninjaC.setScale(-1, 1);
                        ninjaC.setOrigin(20, 0);
                        if(n.veloc[game.frame].x > 0 && game.canJump(n)){
                            dustC.setTexture(dust[dpt]);
                            dustC.setPosition(gridSize*(n.pos[game.frame].x-game.nRad)-5,gridSize*(n.pos[game.frame].y-game.nRad)+10);
                            main.draw(dustC);
                            dustC.setPosition(gridSize*(n.pos[game.frame].x-game.nRad)+5,gridSize*(n.pos[game.frame].y-game.nRad)+10);
                            main.draw(dustC);

                        }
                        break;
                    case 'D':
                        ninjaC.setScale(1, 1);
                        ninjaC.setOrigin(0, 0);
                        if(n.veloc[game.frame].x < 0 && game.canJump(n)){
                            dustC.setTexture(dust[dpt]);
                            dustC.setPosition(gridSize*(n.pos[game.frame].x-game.nRad)-5,gridSize*(n.pos[game.frame].y-game.nRad)+10);
                            main.draw(dustC);
                            dustC.setPosition(gridSize*(n.pos[game.frame].x-game.nRad)+5,gridSize*(n.pos[game.frame].y-game.nRad)+10);
                            main.draw(dustC);
                        }
                        break;
                    default:
                        if(n.veloc[game.frame].x < 0){
                            ninjaC.setScale(-1, 1);
                            ninjaC.setOrigin(20, 0);
                        }else{
                            ninjaC.setScale(1, 1);
                            ninjaC.setOrigin(0, 0);
                        }
                        break;
                }
                ninjaC.setPosition(gridSize*(n.pos[game.frame].x-game.nRad),gridSize*(n.pos[game.frame].y-game.nRad));
                main.draw(ninjaC);
                if(n.shurikenTime!=-1 && n.shurikenTime > game.frame - 5 && n.shurikenTime <= game.frame){
                lines[0]=sf::Vector2f(n.pos[n.shurikenTime].x,n.pos[n.shurikenTime].y)*gridSize;
                vector2 e = game.shurikenDemise(n.pos[n.shurikenTime], n.shurikenTarg, n.shurikenTime)*gridSize;
                lines[1]=sf::Vector2f(e.x,e.y);
                sf::Color c(255,255,255,(5 - n.shurikenTime + game.frame)*50);
                lines[0].color=c;
                lines[1].color=c;
                main.draw(lines);
                }
            }
            if(firing){
                sf::Vector2i localPosition = sf::Mouse::getPosition(main);
                targ.setPosition(localPosition.x-5, localPosition.y-5);
                lines[0]=sf::Vector2f(game.selected->pos[game.frame].x,game.selected->pos[game.frame].y)*gridSize;
                vector2 e = game.shurikenDemise(game.selected->pos[game.frame], vector2(localPosition.x,localPosition.y)/gridSize, game.frame)*gridSize;
                lines[1]=sf::Vector2f(e.x,e.y);
                main.draw(lines);
            }
            speedac += speed;
            while(speedac < -1){
                speedac += 1;
                --game.frame;
                if(game.frame < 0){
                    speed = 0;
                    ospeed = 1;
                    game.frame = 0;
                    if(winlap) speed = 1;
                }
            }
            while(speedac > 1){
                speedac -= 1;
                game.update();
                if(game.frame == game.frames - 1){
                    speed = 0;
                    ospeed = -1;
                }
            }
            if(game.selected != 0 && game.selected->shurikenTime == game.frame+1){
                game.selected->shurikenTime = game.frames+1;
            }
            for(enemy* e : game.enemies){
                if(e->killFrame > game.frame){
                    e->onDeath(game.frames+1, &game);
                    e->killFrame = game.frames+1;
                }
            }
            main.draw(bottomPanel);
            main.draw(timeline);
            timeIndicator.setPosition(20+game.frame, 410);
            if(speed > 0 || (speed == 0 && ospeed > 0)){
                timeIndicator.setFillColor(sf::Color::Yellow);
                timeIndicator.setOutlineColor(sf::Color::Yellow);
            }else{
                timeIndicator.setFillColor(sf::Color::Red);
                timeIndicator.setOutlineColor(sf::Color::Red);
            }
            if(winlap){
                main.draw(victory);
                if(blinkClock.getElapsedTime().asMilliseconds() % 500 > 250) main.draw(replay);
            }
            main.draw(timeIndicator);
            main.draw(t);
            if(game.selected != 0 && game.spiked(*game.selected)){
                game.selected = 0;
            }
        }
        // draw everything here...
        // window.draw(...);
        
        // end the current frame
        main.display();
    }
    std::cout << "Hello, World!\n";
    return 0;
}

