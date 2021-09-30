
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include "map.h"
#include "dwarf.h"
#include <iostream>
#include <math.h>
#include <sstream>

#define killTrapCost 2000
#define shinyCost 1000
#define stickyTrapCost 500
#define emptyCost 100


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
unsigned int generateValue(unsigned int x,unsigned int y){
    float xf = x/50.f;
    float yf = y/50.f;
    float p = 1 - sqrt(2*((0.5-xf)*(0.5-xf) + (0.5-yf)*(0.5-yf)));
    int val = rand()%75 + 35; //Range is 35 - 110
    return floor(val*p);
}
bool isOn(unsigned int x,unsigned int y,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){
    return x1 < x && x < x2 && y1 < y && y < y2;
}
inline void BLOW_CRAP_UP(unsigned int x,unsigned y,map* m){
    m->setAt(position(x-1,y-1), new empty);
    m->setAt(position(x,y-1), new empty);
    m->setAt(position(x+1,y-1), new empty);
    
    m->setAt(position(x-1,y), new empty);
    m->setAt(position(x,y), new empty);
    m->setAt(position(x+1,y), new empty);
    
    m->setAt(position(x-1,y+1), new empty);
    m->setAt(position(x,y+1), new empty);
    m->setAt(position(x+1,y+1), new empty);
}
inline void makeMap(map& m){
    for(unsigned int x = 1;x < 49;++x){
        for(unsigned int y = 1;y < 49;++y){
            unsigned int i = rand() & 7;
            if(i < 2){
                m.setAt(position(x,y), new empty);
            }else if(i < 7){
                m.setAt(position(x,y), new rock(rand()%23+12, generateValue(x, y)));
            }
        }
    }
    BLOW_CRAP_UP(2, 2, &m);
    BLOW_CRAP_UP(47, 2, &m);
    BLOW_CRAP_UP(47, 47, &m);
    BLOW_CRAP_UP(2, 47, &m);
    
    new dwarf(m.getAt(position(1,1)), &m);
    new dwarf(m.getAt(position(48,48)), &m);
    new dwarf(m.getAt(position(1,48)), &m);
    new dwarf(m.getAt(position(48,1)), &m);
}
int main()
{
    sf::Music main;
    main.OpenFromFile("Theme 2.aif");
    main.SetLoop(true);
    main.SetVolume(25.f);
    main.Play();
    position spawns[] = {position(1,1),position(48,1),position(48,48),position(1,48)};
    unsigned int dwarfSpawnTime = 250;
    unsigned int dwarfSpawnDelay = 500;
    unsigned int dwarfSpawnPosition = 0;
    sf::RenderWindow App(sf::VideoMode(640, 500, 32), "Dwarven Isolation");
    unsigned int stepSize = 250;
    unsigned int failOMeter = 0;
    const char* firstnames[] = {"Urist","Glarg","Kruth","Luric"};
    unsigned int firstnamesize = 3;
    const char* lastnames[] = {"McBeer","Dwarfson","Goldminer","Diamondbeard","Carpslayer"};
    unsigned int lastnamesize = 5;
    unsigned int firstName1;
    unsigned int firstName2;
    unsigned int lastName1;
    unsigned int lastName2;
    imageManager imgMgr;
    enum{
        STEP, PAUSED, MENU, EPIC_FAIL, HELP
    } gamestate = MENU;
    enum{
        NONE, KILLTRAP, STICKTRAP, SHINY, MINERAL_REMOVE
    } placing = NONE;
    unsigned int stepsLeft = stepSize;
    unsigned int dwarfLeft = dwarfSpawnTime + dwarfSpawnDelay;
    map* m = new map(50,50);
    sf::Sprite killtrapButton = sf::Sprite(imgMgr.imgs[imgMgr.deadlyButton],sf::Vector2f(510,70));
    sf::Sprite shinyButton = sf::Sprite(imgMgr.imgs[imgMgr.shinyButton],sf::Vector2f(510,105));
    sf::Sprite sticktrapButton = sf::Sprite(imgMgr.imgs[imgMgr.stickyButton],sf::Vector2f(510,140));
    sf::Sprite mineralremoveButton = sf::Sprite(imgMgr.imgs[imgMgr.emptyButton],sf::Vector2f(510,175));
    sf::Sprite continueButton = sf::Sprite(imgMgr.imgs[imgMgr.doneButton],sf::Vector2f(510,465));
    sf::Sprite menuButton = sf::Sprite(imgMgr.imgs[imgMgr.menuButton],sf::Vector2f(160,440));
    sf::Sprite menu = sf::Sprite(imgMgr.imgs[imgMgr.menu],sf::Vector2f(0,0));
    sf::Clock mainClock;

    // Create the main window
    App.SetFramerateLimit(60);
    App.Clear();
    //m.fullRender(App, imgMgr);
    // Start game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
			
            // Escape key : exit
            if (Event.Type == sf::Event::KeyPressed){
                if(Event.Key.Code == sf::Key::Escape)
                    App.Close();
            }
			if(Event.Type == sf::Event::MouseButtonPressed){
                if(gamestate == EPIC_FAIL || gamestate == HELP){
                    if(isOn(Event.MouseButton.X, Event.MouseButton.Y, 160, 440, 480, 490)){
                        gamestate = MENU;
                    }
                }else
                if(gamestate == MENU){
                    unsigned int mx = Event.MouseButton.X;
                    unsigned int my = Event.MouseButton.Y;
                    if(isOn(mx, my, 320, 260, 560, 310)){
                        stepSize = 250;
                        stepsLeft = stepSize;
                        dwarfLeft = dwarfSpawnDelay + dwarfSpawnTime;
                        dwarfSpawnPosition = 0;
                        delete m;
                        m = new map(50,50);
                        makeMap(*m);
                        gamestate = STEP;
                    }else if(isOn(mx, my, 320, 320, 560, 370)){
                        stepSize = 500;
                        stepsLeft = stepSize;
                        dwarfLeft = dwarfSpawnDelay + dwarfSpawnTime;
                        dwarfSpawnPosition = 0;
                        delete m;
                        m = new map(50,50);
                        makeMap(*m);
                        gamestate = STEP;
                    }else if(isOn(mx, my, 320, 380, 560, 430)){
                        stepSize = 750;
                        stepsLeft = stepSize;
                        dwarfLeft = dwarfSpawnDelay + dwarfSpawnTime;
                        dwarfSpawnPosition = 0;
                        delete m;
                        m = new map(50,50);
                        makeMap(*m);
                        gamestate = STEP;
                    }else if(isOn(mx, my, 320, 440, 560, 490)){
                        gamestate = HELP;
                    }
                }
                if(gamestate == PAUSED){
                    unsigned int x = Event.MouseButton.X;
                    unsigned int y = Event.MouseButton.Y;
                    if(isOn(x, y, 510, 70, 630, 95)){
                        placing = KILLTRAP;
                    }else if(isOn(x, y, 510, 105, 630, 130)){
                        placing = SHINY;
                    }else if(isOn(x, y, 510, 140, 630, 165)){
                        placing = STICKTRAP;
                    }else if(isOn(x, y, 510, 175, 630, 200)){
                        placing = MINERAL_REMOVE;
                    }else if(isOn(x, y, 510, 465, 630, 490)){
                        placing = NONE;
                        stepsLeft = stepSize;
                        gamestate = STEP;
                    }
                    if(x > 500) continue;
                    x /= 10;
                    y /= 10;
                    position p = position(x,y);
                    if(m->getAt(p)->walk() == -1) continue;
                    if(m->isInSight(p)) continue;
                    if(m->getAt(p)->t == place::DEAD) continue; //Respect the dead, please
                    if(placing == KILLTRAP && m->getAt(p)->t != place::DEADLY && m->money > killTrapCost){
                        m->setAt(p, new killTrap);
                        m->needsUpdate.push_back(p);
                        m->money -= killTrapCost;
                    }
                    if(placing == SHINY && m->getAt(p)->t != place::SHINY && m->money > shinyCost){
                        m->setAt(p, new shiny);
                        m->needsUpdate.push_back(p);
                        m->money -= shinyCost;
                    }
                    if(placing == STICKTRAP && m->getAt(p)->t != place::STICKY && m->money > stickyTrapCost){
                        m->setAt(position(x,y), new stickyTrap);
                        m->needsUpdate.push_back(position(x,y));
                        m->money -= stickyTrapCost;
                    }
                    if(placing == MINERAL_REMOVE && m->getAt(p)->t != place::EMPTY && m->money > emptyCost){
                        m->setAt(position(x,y), new empty);
                        m->needsUpdate.push_back(position(x,y));
                        m->money -= emptyCost;
                    }
                }
            }
            // Resize event : adjust viewport
        }
        sf::Input const& in = App.GetInput();
        unsigned int mx = in.GetMouseX();
        unsigned int my = in.GetMouseY();
        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

        // Clear color buffer		
        // Your drawing here...
        if(gamestate == STEP){
            m->update();
            --stepsLeft;
            --dwarfLeft;
            if(dwarfLeft == 0){
                dwarfLeft = dwarfSpawnTime;
                new dwarf(m->getAt(spawns[dwarfSpawnPosition]),m);
                if(++dwarfSpawnPosition == 4) dwarfSpawnPosition = 0;
            }
            if(stepsLeft == 0){
                gamestate = PAUSED;
            }
            if(m->loser){
                m->fullRender(App, &imgMgr);
                lastName1 = rand() % lastnamesize;
                lastName2 = rand() % lastnamesize;
                firstName1 = rand() % firstnamesize;
                firstName2 = rand() % firstnamesize;
                gamestate = EPIC_FAIL;
                failOMeter = 0;
                imgMgr.sounds[3].Play();
            }
        }
        if(gamestate == STEP || gamestate == PAUSED){
            m->fullRender(App, &imgMgr);
            if(dwarfLeft <= dwarfSpawnTime){
                if(fmod(mainClock.GetElapsedTime(),1) < 0.5){
                    App.Draw(sf::Sprite(imgMgr.imgs[imgMgr.dwarf],sf::Vector2f(spawns[dwarfSpawnPosition].x*10,spawns[dwarfSpawnPosition].y*10)));
                }
            }
            std::stringstream s;
            s << "Score: " << m->score;
            App.Draw(sf::Shape::Rectangle(500, 0, 640, 500, sf::Color::Black));
            sf::String str(s.str());
            str.SetSize(20);
            str.SetPosition(510, 10);
            App.Draw(str);
            s.str("");
            s << "Money: " << (m->money/10);
            str.SetText(s.str());
            str.SetPosition(510, 40);
            App.Draw(str);
            App.Draw(killtrapButton);
            App.Draw(shinyButton);
            App.Draw(sticktrapButton);
            App.Draw(continueButton);
            App.Draw(mineralremoveButton);
        }
        if(gamestate == HELP){
            App.Clear();
            std::stringstream help("You, the player, have locked several dwarves in a mineral-rich room. Like all evil people, your goal is to make lots of money. Obviously, these dwarves explode when they come into contact with one another, and thus you must keep them ALONE. Unfortunately, due to your powerful greed, you cannot keep yourself from putting more and more explosives dwarves in this confined space. This game is played in two phases. In the first, dwarves run around and do their thing, and earn you lots of money. In the second, you can place traps and lures in order to keep the dwarves from meeting, and, if necessary, kill them. Be forewarned though, because dwarves will scream and alert all other dwarves in a huge radius if they find a body.");
            unsigned int x = 30;
            unsigned int y = 50;
            std::string word;
            sf::String s;
            s.SetSize(20);
            while(true){
                help >> word;
                if(!help) break;
                s.SetText(word);
                s.SetPosition(x, y);
                sf::FloatRect rect = s.GetRect();
                if(rect.Right > 630){
                    y = rect.Bottom + 5;
                    s.SetPosition(10, y);
                    rect = s.GetRect();
                }
                App.Draw(s);
                x = rect.Right + 5;
            }
        }
        if(gamestate == STEP){
            float x = 1-stepsLeft/float(stepSize);
            App.Draw(sf::Shape::Rectangle(500, x*500, 505, 500, sf::Color::White));
        }
        if(gamestate == PAUSED){ //HIGHLIGHT, TIMELAPSE
            sf::Color highlight(255,255,255,64);
            if(isOn(mx, my, 510, 70, 630, 95)){
                App.Draw(sf::Shape::Rectangle(510, 70, 630, 95, highlight));
            }else if(isOn(mx, my, 510, 105, 630, 130)){
                App.Draw(sf::Shape::Rectangle(510, 105, 630, 130, highlight));
            }else if(isOn(mx, my, 510, 140, 630, 165)){
                App.Draw(sf::Shape::Rectangle(510, 140, 630, 165, highlight));
            }else if(isOn(mx, my, 510, 175, 630, 200)){
                App.Draw(sf::Shape::Rectangle(510, 175, 630, 200, highlight));
            }else if(isOn(mx, my, 510, 465, 630, 490)){
                App.Draw(sf::Shape::Rectangle(510, 465, 630, 490, highlight));
            }
            highlight = sf::Color(255,255,0,64);
            if(placing == KILLTRAP){
                App.Draw(sf::Shape::Rectangle(510, 70, 630, 95, highlight));
            }else if(placing == SHINY){
                App.Draw(sf::Shape::Rectangle(510, 105, 630, 130, highlight));
            }else if(placing == STICKTRAP){
                App.Draw(sf::Shape::Rectangle(510, 140, 630, 165, highlight));
            }else if(placing == MINERAL_REMOVE){
                App.Draw(sf::Shape::Rectangle(510, 175, 630, 200, highlight));
            }
        }
        if(gamestate == EPIC_FAIL){
            failOMeter += 5;
            if(failOMeter <= 640){
                sf::Shape n = sf::Shape::Rectangle(0,0,640,500,sf::Color(8,8,4));
                n.SetBlendMode(sf::Blend::Add);
                App.Draw(n);
                for(unsigned int i = 0;i < 50;++i){
                    float ang = rand()/float(RAND_MAX)*2*M_PI;
                    unsigned int x1 = m->loser->pos->pos.x*10;
                    unsigned int y1 = m->loser->pos->pos.y*10;
                    unsigned int x2 = x1 + sin(ang)*failOMeter;
                    unsigned int y2 = y1 + cos(ang)*failOMeter;
                    sf::Shape sdfa = sf::Shape::Line(x1, y1, x2, y2, 3, sf::Color::Yellow);
                    sdfa.SetBlendMode(sf::Blend::Add);
                    App.Draw(sdfa);
                }
            }else{
                App.Clear(sf::Color(255,255,255));
                unsigned int alpha = (failOMeter < 640+255*10)?255-(failOMeter-640)/10:0;
                sf::Color drawColor(alpha,alpha,alpha);
                std::stringstream text;
                text << "Total Score: " << m->score << "\n\n"
                    << "Remaining Money: " << m->money/10 << "\n\n"
                    << "Money Spent: " << (m->score/10-m->money/10) << "\n\n"
                    << "Screams Heard: " << m->screams << "\n\n"
                    << "Dwarves Killed: " << m->kills << "\n\n"
                    << "Names of Dwarves who Met: " << firstnames[firstName1] << " " << lastnames[lastName1] << " & " << firstnames[firstName2] << " " << lastnames[lastName2];
                sf::String str(text.str());
                str.SetColor(drawColor);
                str.SetSize(20);
                str.SetPosition(10, 50);
                App.Draw(str);
                menuButton.SetColor(sf::Color(255,255,255,255-alpha));
                App.Draw(menuButton);
                sf::Color highlight(255,255,255,64);
                if(isOn(mx, my, 160, 440, 480, 490)){
                    App.Draw(sf::Shape::Rectangle(160, 440, 480, 490, highlight));
                }
            }
        }
        if(gamestate == HELP){
            menuButton.SetColor(sf::Color(255,255,255));
            App.Draw(menuButton);
            sf::Color highlight(255,255,255,64);
            if(isOn(mx, my, 160, 440, 480, 490)){
                App.Draw(sf::Shape::Rectangle(160, 440, 480, 490, highlight));
            }
        }
        if(gamestate == MENU){
            App.Draw(menu);
            sf::Color highlight(255,255,255,64);
            if(isOn(mx, my, 320, 260, 560, 310)){
                App.Draw(sf::Shape::Rectangle(320, 260, 560, 310, highlight));
            }else if(isOn(mx, my, 320, 320, 560, 370)){
                App.Draw(sf::Shape::Rectangle(320, 320, 560, 370, highlight));
            }else if(isOn(mx, my, 320, 380, 560, 430)){
                App.Draw(sf::Shape::Rectangle(320, 380, 560, 430, highlight));
            }else if(isOn(mx, my, 320, 440, 560, 490)){
                App.Draw(sf::Shape::Rectangle(320, 440, 560, 490, highlight));
            }
        }
        // Finally, display rendered frame on screen
        App.Display();
    }
	
    return EXIT_SUCCESS;
}
