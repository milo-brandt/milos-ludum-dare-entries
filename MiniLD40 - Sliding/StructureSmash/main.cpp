#include "gameInstance.h"
#include "levelBuild.h"

int main (int argc, const char * argv[])
{
    // Create the main window
    sf::Window window(sf::VideoMode(800, 600), "SS");
    window.setFramerateLimit(60);
    glClearColor(0, 0.6, 1, 1);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 20, 0, 15, -1, 1);
	glMatrixMode(GL_MODELVIEW);
    int levelNum = 0;

    gameInstance* gam = 0;
    levelBuild* g = 0;
    float r = 0;
    while (window.isOpen())
    {
        if(g == 0){
            if(levelNum == 0){
                std::vector<structure*> s;
                s.push_back(structure::makeCanyon(7, 30));
                s.push_back(structure::makeWinZone(10));
                g= new levelBuild(window, s);
                g->anchored = 0;
            }
            if(levelNum == 1){
            std::vector<structure*> s;
            s.push_back(structure::makeCanyon(7, 50));
            s.push_back(structure::makeJump(20));
            s.push_back(structure::makeSlope(20));
            s.push_back(structure::makeWinZone(10));
            s.push_back(structure::makeBack(20));
                g= new levelBuild(window, s);
            g->anchored = 2;
        }
            if(levelNum == 2){
                std::vector<structure*> s;
                s.push_back(structure::makeJump(20));
                s.push_back(structure::makeCanyon(7, 3));
                s.push_back(structure::makeSlopeUp(6));
                s.push_back(structure::makeJump(1));
                s.push_back(structure::makeCanyon(7, 20));
                s.push_back(structure::makeCanyon(7, 3));
                s.push_back(structure::makeWinZone(10));
                s.push_back(structure::makeCanyon(7, 10));
                s.push_back(structure::makeBack(20));
                g= new levelBuild(window, s);

                g->anchored = 4;

            }
            if(levelNum == 3){
                std::vector<structure*> s;
                s.push_back(structure::makeCanyon(7, 3));
                s.push_back(structure::makeSlopeUp(3));
                s.push_back(structure::makeCanyon(7, 3));
                s.push_back(structure::makeCanyon(7, 3));
                s.push_back(structure::makeJump(6)); 
                s.push_back(structure::makeCanyon(7, 10));
                s.push_back(structure::makeCanyon(7, 3));
                s.push_back(structure::makeCanyon(7, 3));
                s.push_back(structure::makeSlope(3));
                s.push_back(structure::makeCanyon(7, 3));
                s.push_back(structure::makeWinZone(10));
                s.push_back(structure::makeCanyon(7, 10));
                s.push_back(structure::makeBack(20));
                g= new levelBuild(window, s);
                
                g->anchored = 4;
                
            }
            if(levelNum == 4){
                std::vector<structure*> s;
                s.push_back(structure::makeSlope(5));
                s.push_back(structure::makeSlope(5));
                s.push_back(structure::makeCanyon(7, 9));
                s.push_back(structure::makeSlope(5));
                s.push_back(structure::makeSlopeUp(10));
                s.push_back(structure::makeSlope(5));
                s.push_back(structure::makeCanyon(7, 18));
                s.push_back(structure::makeJump(10));
                s.push_back(structure::makeJump(20));
                s.push_back(structure::makeSlopeUp(12));
                s.push_back(structure::makeWinZone(10));
                s.push_back(structure::makeCanyon(7, 10));
                s.push_back(structure::makeBack(20));
                g= new levelBuild(window, s);
                
                g->anchored = 3;
                
            }
            if(levelNum == 5){
                std::vector<structure*> s;
                s.push_back(structure::makeJump(35));
                s.push_back(structure::makeCanyon(7, 30));
                s.push_back(structure::makeSlopeUp(3));
                s.push_back(structure::makeJump(13));
                s.push_back(structure::makeCanyon(7, 3));


                s.push_back(structure::makeBack(10));
                s.push_back(structure::makeWinZone(4));
                s.push_back(structure::makeCanyon(7, 10));
                s.push_back(structure::makeBack(20));
                g= new levelBuild(window, s);
                
                g->anchored = 4;
                
            }
            if(levelNum == 6){
                std::vector<structure*> s;
                s.push_back(structure::makeJump(125));
                s.push_back(structure::makeCanyon(7, 50));
                s.push_back(structure::makeBack(40));
                s.push_back(structure::makeCanyon(7, 50));
                s.push_back(structure::makeWinZone(20));
                s.push_back(structure::makeBack(100));
                g= new levelBuild(window, s);
                
                g->anchored = 2;
                
            }
            if(levelNum >= 7){
                std::vector<structure*> s;
                int canyonsLeft = rand()%levelNum+1;
                int upsLeft = rand()%(levelNum - 4);
                int jumpsLeft = canyonsLeft+upsLeft-1+rand()%5;
                int downsLeft = rand()%(levelNum-2);
                while(true){
                    int tot = canyonsLeft+upsLeft+jumpsLeft+downsLeft;
                    if(tot == 0) break;
                    int x = rand()%tot;
                    if(x < canyonsLeft){
                        --canyonsLeft;
                        s.push_back(structure::makeCanyon(8, rand()%10+10));
                    }else if(x < canyonsLeft + upsLeft){
                        --upsLeft;
                        s.push_back(structure::makeSlopeUp(rand()%3+2));
                    }else if(x < canyonsLeft + upsLeft + jumpsLeft){
                        --jumpsLeft;
                        s.push_back(structure::makeJump(rand()%10+5));
                    }else{
                        --downsLeft;
                        s.push_back(structure::makeSlope(rand()%3+5));
                    }
                }
                s.push_back(structure::makeWinZone(20));
                s.push_back(structure::makeBack(20));
                g= new levelBuild(window, s);
                
                g->anchored = 2;
            }
        }
        if(gam == 0){
    	// Process events
            if(g->update()){ 
                gam=g->game();
            }
        }else{
            int o = gam->update();
            if(o == 1){
                delete gam;
                gam = 0;
            }
            if(o== 2){
                delete gam;
                gam = 0;
                delete g;
                g = 0;
                ++levelNum;
            }
        }

    }

	return EXIT_SUCCESS;
}
