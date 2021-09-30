//
//  leveldata.h
//  LD21
//
//  Created by Milo Brandt on 8/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include <vector>
#include <SFML/Graphics.hpp>
#ifndef LEVELDATA_H
#define LEVELDATA_H
struct building{
    int pop;
    int x;
    int y;
    int width;
    int height;
    int spawnX;
    int spawnY;
    bool spawning;
    building(int x,int y,int spawnX,int spawnY,int width,int height,int pop):x(x),y(y),width(width),height(height),pop(pop),spawnX(spawnX),spawnY(spawnY),spawning(false){}
    sf::Color getColor();
};
struct roadRow{
    int x1;
    int y1;
    int x2;
    int y2;
};
struct car{
    int x;
    int y;
    int r;
    int g;
    int b;
    char transition;
    float transitionProgress;
    car(int x,int y,char transition,float transitionProgress):x(x),y(y),transition(transition),transitionProgress(transitionProgress),r((rand()&128)+64),g((rand()&128)+64),b((rand()&128)+64){}
    float getX();
    float getY();
    sf::Color getColor();
};
#include <list>
class levelData{
public:
    unsigned char* roadData;
    static const char downRoadUp = 1;
    static const char downRoadDown = 2;
    static const char downRoad = downRoadUp | downRoadDown;
    static const char rightRoadLeft = 4;
    static const char rightRoadRight = 8;
    static const char rightRoad = rightRoadLeft | rightRoadRight; 
    static const char occupied = 16;
    float deadlyWave;
    unsigned char* fastestWayOut;
    int width;
    int height;
    int escaped;
    int alive;
    int buildingsEvacuated;
    int carsLostToWave;
    int totalCars;
    float deadlyWaveSpeed;
    sf::Image carimg;
    std::list<car> cars;
    std::vector<building> buildings;
    levelData(int width,int height);
    ~levelData();
    void draw(sf::RenderWindow&);
    void generateFastest();
    bool routeExists(int x,int y,char dir);
    bool hasRoad(int x,int y,char dir);
    void drawHover(sf::RenderWindow&,int mouseX,int mouseY);
    void handleClick(int mouseX,int mouseY);
    roadRow fromPoint(int x,int y,char dir);
    static void generateCity(levelData&);
    bool update();
};
#endif