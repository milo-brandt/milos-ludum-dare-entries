//
//  titlePage.cpp
//  LD21
//
//  Created by Milo Brandt on 8/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "titlePage.h"
#include <sstream>
namespace titlePage {
    void initStuff(){
        titleImage.LoadFromFile("Title.png");
        creditsButton.LoadFromFile("CreditsButton.png");
        helpButton.LoadFromFile("HelpButton.png");
        playButton.LoadFromFile("PlayButton.png");
        backButton.LoadFromFile("back.png");
        creditsPage.LoadFromFile("CreditPage.png");
        playButtonHard.LoadFromFile("PlayButtonHard.png");
        statsPage.LoadFromFile("statsScreen.png");
    }
    void setPageType(int p){
        pageType = p;
    }
    void drawTitle(sf::RenderWindow &r,int mX,int mY,levelData& l){
        sf::Sprite back;
        if(pageType == 0){
            back.SetImage(titleImage);
            back.SetPosition(0, 0);
            r.Draw(back);
            back = sf::Sprite();
            back.SetScale(0.75, 0.75);
            back.SetImage(playButton);
            back.SetPosition(10, 200);
            if(mX > 10 && mX < 150 && mY > 200 && mY < 275){
                back.SetColor(sf::Color(255,255,0,255));
                l.deadlyWaveSpeed = 0.003;
            }else back.SetColor(sf::Color(200,200,200));
            r.Draw(back);
            back.SetImage(playButtonHard);
            back.SetPosition(10, 285);
            if(mX > 10 && mX < 150 && mY > 285 && mY < 360){
                back.SetColor(sf::Color(255,128,0,255));
                l.deadlyWaveSpeed = 0.006; //DEADLY
            }else back.SetColor(sf::Color(200,200,200));
            r.Draw(back);
            back.SetImage(helpButton);
            back.SetPosition(10, 370);
            if(mX > 10 && mX < 150 && mY > 370 && mY < 445) back.SetColor(sf::Color(255,255,0,255));
            else back.SetColor(sf::Color(200,200,200));
            r.Draw(back);
            back.SetImage(creditsButton);
            back.SetPosition(10, 455);
            if(mX > 10 && mX < 150 && mY > 455 && mY < 530) back.SetColor(sf::Color(255,255,0,255));
            else back.SetColor(sf::Color(200,200,200));
            r.Draw(back);
        }
        if(pageType == 1){
            sf::String Text("Instructions can be found included in the download or\n online. Haven't got around to integratin' 'em yet.",sf::Font::GetDefaultFont(),25);
            Text.SetColor(sf::Color::Black);
            r.Draw(Text);
        }
        if(pageType == 2){
            back.SetImage(creditsPage);
            back.SetPosition(0, 0);
            r.Draw(back);
        }
        if(pageType == 3){
            back.SetImage(statsPage);
            r.Draw(back);
            std::stringstream str;
            str << "Rescued: " << l.escaped << " cars (" << floorf(l.escaped/(l.totalCars/100.0) + 0.5) << "% of population)\n"
                << "Lost: " << (l.totalCars - l.escaped) << " cars (" << (100-floorf(l.escaped/(l.totalCars/100.0) + 0.5)) << "% of population)\n"
                << "     -Cars Lost while Driving: " << l.carsLostToWave << "\n"
                << "     -Cars Lost in Buildings: " << ((l.totalCars - l.escaped) - l.carsLostToWave) << "\n"
                << "Buildings Fully Evacuated: " << l.buildingsEvacuated << " (" << floorf(l.buildingsEvacuated/(l.buildings.size()/100.0) + 0.5) << "%)\n";
            sf::String Text(str.str(),sf::Font::GetDefaultFont(),25);
            Text.SetColor(sf::Color::White);
            sf::FloatRect rz = Text.GetRect();
            Text.SetPosition(320 - rz.GetWidth()/2, 75);
            r.Draw(Text);
        }
        if(pageType == 1 || pageType == 2 || pageType == 3){
            back = sf::Sprite();
            back.SetImage(backButton);
            back.SetPosition(480, 477);
            if(mX > 480 && mX < 630 && mY > 477 && mY < 552) back.SetColor(sf::Color(255,255,0,255));
            else back.SetColor(sf::Color(200,200,200));
            r.Draw(back);
        }
    }
    bool handleClick(int mX,int mY){
        if(pageType == 0){
            if(mX > 10 && mX < 150 && mY > 200 && mY < 275) return true;
            if(mX > 10 && mX < 150 && mY > 285 && mY < 360) return true;
            if(mX > 10 && mX < 150 && mY > 370 && mY < 445){
                pageType = 1;
                return false;
            }
            if(mX > 10 && mX < 150 && mY > 455 && mY < 530){
                pageType = 2;
                return false;
            }
        }
        if(pageType == 1 || pageType == 2 || pageType == 3){
            if(mX > 480 && mX < 630 && mY > 477 && mY < 552){
                pageType = 0;
                return false;
            }
        }
        return false;
    }
}