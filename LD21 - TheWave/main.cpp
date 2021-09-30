
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "titlePage.h"
////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create main window
    sf::RenderWindow App(sf::VideoMode(640, 562), "SFML Graphics");
    App.SetFramerateLimit(30); //Wouldn't wanna go too fast!
    sf::View MainView(sf::FloatRect(0, 562, 640, 0));
    srand(clock());
    levelData l(20,16);
    // Start game loop
    titlePage::initStuff();
    sf::Music Music1;
    Music1.OpenFromFile("bgm.aif");
    Music1.Play();
    Music1.SetLoop(true);
    bool playingGame = false;
    int fadeProgress = 0;
    bool fading = false;
    int fadeDir = 4;
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
            if(Event.Type == sf::Event::KeyPressed){
                if(Event.Key.Code == sf::Key::Q){
                    sf::Image img = App.Capture();
                    img.SaveToFile(std::string("/Screenshot.png"));
                }
            }
            if(Event.Type == sf::Event::MouseButtonPressed){
                if(playingGame){
                    l.handleClick(Event.MouseButton.X, Event.MouseButton.Y);
                }else{
                    playingGame = titlePage::handleClick(Event.MouseButton.X, Event.MouseButton.Y);
                    if(playingGame){
                        levelData::generateCity(l);
                        l.generateFastest();
                        playingGame = false;
                        fading = true;
                        fadeDir = 8;
                    }
                }
            }
        }
        const sf::Input& in = App.GetInput();
        // Clear screen
        App.Clear(sf::Color::White);

        if(playingGame){
            if(l.update()){
                titlePage::setPageType(3);
                fading = true;
                fadeDir = 8;
            }
            l.draw(App);
            l.drawHover(App, in.GetMouseX(), in.GetMouseY());
        }else{
            titlePage::drawTitle(App,in.GetMouseX(),in.GetMouseY(),l);
        } 
        if(fading){
            fadeProgress += fadeDir;
            if(fadeProgress >= 256){
                fadeDir = -fadeDir;
                playingGame = !playingGame;
            }
            if(fadeProgress <= 0){
                fading = false;
                fadeProgress = 0;
            }
            App.Draw(sf::Shape::Rectangle(0, 0, 640, 562, sf::Color(0,0,0,(fadeProgress>=255)?255:fadeProgress)));
        }
        // Or, if you want to do it after the construction :
        // Finally, display the rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}
