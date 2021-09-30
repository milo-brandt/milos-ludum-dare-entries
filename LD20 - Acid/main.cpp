
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "gameModule.h"
#include "titleScreen.h"
#include "intro.h"
#include <math.h>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define LEVEL_NUM 5
int main()
{
	const char* levelNames[LEVEL_NUM] = {"test.txt","test3.txt","test2.txt","test4.txt","test5.txt"};
	int levelImgs[LEVEL_NUM] = {lvl1_sprite,lvl2_sprite,-1,-1,-1};
	int levelNum = 0;
    // Create the main window
    sf::Window App(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Acid");
	App.SetFramerateLimit(30);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIN_WIDTH, 0, WIN_HEIGHT, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LINE_SMOOTH);
	glLoadIdentity();
	imageManager* imageMain = new imageManager();
	int animprogress;
	gameModule* x = 0;
	titleScreen* title = new titleScreen();
	intro* introduction = 0;
	int img = -1;
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
                if(Event.Key.Code == sf::Key::Escape){
					App.Close();
				}else if(x != 0){
					if(x->handle(Event)){
						delete x;
						if(levelImgs[levelNum] != -1){
							x = 0;
							img = levelImgs[levelNum];
						}else{
							x = new gameModule(App,levelNames[levelNum]);
						}
					}
				}else if(introduction != 0){
					introduction->keyPress();
				}else if(img != -1 && img != win_sprite){
					img = -1;
					x = new gameModule(App,levelNames[levelNum]);
				}
			}
            // Resize event : adjust viewport
            if (Event.Type == sf::Event::Resized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);
			if(Event.Type == sf::Event::MouseButtonPressed){
				const sf::Input& n = App.GetInput();
				float x = n.GetMouseX();
				float y = n.GetMouseY();
				if(title != 0){
					if(title->handleClick(x,y)){
						delete title;
						title = 0;
						introduction = new intro();
					}
				}
			}
        }
        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();
        // Clear color buffer
        glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		if(title != 0) title->render(imageMain);
		if(introduction != 0) if(introduction->render(imageMain)){
			delete introduction;
			introduction = 0;
			if(levelImgs[levelNum] != -1){
				x = 0;
				img = levelImgs[levelNum];
			}else{
				x = new gameModule(App,levelNames[0]);
			}
		}
		if(img != -1){
			glColor3f(1.f,1.f,1.f);
			imageMain->x[img].Bind();
			glBegin(GL_QUADS);
			
			glTexCoord2f(0.f,1.f);
			glVertex2f(0.f,0.f);
			glTexCoord2f(1.f,1.f);
			glVertex2f(640.f,0.f);
			glTexCoord2f(1.f,0.f);
			glVertex2f(640.f,480.f);
			glTexCoord2f(0.f,0.f);
			glVertex2f(0.f,480.f);
			
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);	
		}
		if(x != 0){
			int st = x->update();
			if(st == 1){
				delete x;
				levelNum++;
				if(levelNum >= LEVEL_NUM){
					x = 0;
					img = win_sprite;
				}else if(levelImgs[levelNum] != -1){
					x = 0;
					img = levelImgs[levelNum];
				}else{
				
					x = new gameModule(App,levelNames[levelNum]);
				}
			}
			if(st == 2){
				delete x;
				if(levelImgs[levelNum] != -1){
					x = 0;
					img = levelImgs[levelNum];
				}else{
					x = new gameModule(App,levelNames[levelNum]);
				}
			}
			if(x != 0){
				x->render(imageMain);
			}
		}
        // Your drawing here...
        // Finally, display rendered frame on screen
        App.Display();
    }
	
    return EXIT_SUCCESS;
}
