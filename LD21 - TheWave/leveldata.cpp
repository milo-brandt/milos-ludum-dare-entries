//
//  leveldata.cpp
//  LD21
//
//  Created by Milo Brandt on 8/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "leveldata.h"
#define popDensity 1
#include "titlePage.h"

levelData::levelData(int width,int height):width(width),height(height),roadData(new unsigned char[width*height]),fastestWayOut(new unsigned char[width*height]){
    for(int i = 0;i < width*height;++i){
        roadData[i] = 0;
        fastestWayOut[i] = 'O';
    }
    escaped = 0;
    deadlyWave = width;
    alive = width*height*popDensity;
    carimg.LoadFromFile("car.png");
}
levelData::~levelData(){
    delete[] roadData;
    delete[] fastestWayOut;
}
#define entrances 3
#define maxBuildingHeight 4
#define maxBuildingWidth 4
bool _isRowOpen(int x,int y,int xmax,int width,int height,bool* open){
    if(y >= height) return false;
    for(int i = x;i < xmax;++i){
        if(!open[i+y*width]) return false;
    }
    return true;
}
bool _isColumnOpen(int x,int y,int ymax,int width,bool* open){
    if(x >= width) return false;
    for(int i = y;i < ymax;++i){
        if(!open[x+i*width]) return false;
    }
    return true;
}
building _placeBuilding(int x,int y,int globalWidth,int globalHeight,bool* open){
    int width = 1;
    int height = 1;
    int desiredWidth = (rand() % maxBuildingWidth) + 1;
    int desiredHeight = (rand() % maxBuildingHeight) + 1;
    if(!_isRowOpen(x, y+height, x+width, globalWidth, globalHeight,open)) desiredHeight = height;
    if(!_isColumnOpen(x+width, y, y+height, globalWidth,  open)) desiredWidth = width;
    while(width < desiredWidth && height < desiredHeight){
        if(rand() & 1){
            ++width;
        }else{
            ++height;
        }
        if(!_isRowOpen(x, y+height, x+width, globalWidth, globalHeight, open)) desiredHeight = height;
        if(!_isColumnOpen(x+width, y, y+height, globalWidth, open)) desiredWidth = width;
    }
    while(height < desiredHeight && _isRowOpen(x, y+height, x+width, globalWidth, globalHeight, open)) ++height;
    while(width < desiredWidth && _isColumnOpen(x+width, y, y+height, globalWidth, open)) ++width;
    for(int px = 0;px < width;++px){
        for(int py = 0;py < height;++py){
            open[(x+px)+(y+py)*globalWidth] = false;
        }
    }
    return building(x, y, (x>globalWidth/2)?x:(x+width),y+((y>globalHeight/2)?(height):(height+1))/2,width, height, width*height*popDensity);
}
#define POS(x,y) ((x)+(y)*width)
inline void _addOption(unsigned char& c,unsigned char option){
    c = c | option;
}
sf::Color building::getColor(){
    if(spawning && pop > 0) return sf::Color(0,64,0);
    if(!spawning && pop > 0) return sf::Color(64,0,0);
    return sf::Color(64,64,64);
}
void levelData::generateCity(levelData& ret){
    int width = ret.width;
    int height = ret.height;
    bool* open = new bool[width*height];
    ret.escaped = 0;
    ret.deadlyWave = width;
    ret.alive = width*height*popDensity;
    ret.buildingsEvacuated = 0;
    ret.carsLostToWave = 0;
    ret.buildings.clear();
    for(int i = 0;i < width*height;++i){
        open[i] = true;
        ret.fastestWayOut[i] = 'O';
        ret.roadData[i] = 0;
    }
    for(int x = 0;x < width;++x){
        for(int y = 0;y < height;++y){
            if(open[x+y*width]){
                ret.buildings.push_back(_placeBuilding(x, y, width, height, open));
            }
        }
    }
    for(int i = 0;i < ret.buildings.size();++i){
        for(int x = 0;x < ret.buildings[i].width;++x){
            _addOption(ret.roadData[POS(ret.buildings[i].x+x,ret.buildings[i].y)],levelData::rightRoadRight | levelData::rightRoadLeft);
            _addOption(ret.roadData[POS(ret.buildings[i].x+x,ret.buildings[i].y+ret.buildings[i].height)],levelData::rightRoadRight | levelData::rightRoadLeft);
        }
        for(int y = 0;y < ret.buildings[i].height;++y){
            _addOption(ret.roadData[POS(ret.buildings[i].x,ret.buildings[i].y+y)],levelData::downRoadDown | levelData::downRoadUp);
            _addOption(ret.roadData[POS(ret.buildings[i].x+ret.buildings[i].width,ret.buildings[i].y+y)],levelData::downRoadDown | levelData::downRoadUp);
        }
    }
    ret.totalCars = width*height*popDensity;
}
#define roadWidth 5
#define gridSize 32
#define lowArrow 0.425
#define highArrow 0.575
#define arrowSpread 3
#define carRadius 4
void levelData::draw(sf::RenderWindow& r){
    for(int i = 0;i < buildings.size();++i){
        r.Draw(sf::Shape::Rectangle(buildings[i].x*gridSize+roadWidth, buildings[i].y*gridSize+roadWidth, (buildings[i].x+buildings[i].width)*gridSize-roadWidth, (buildings[i].y+buildings[i].height)*gridSize-roadWidth, buildings[i].getColor()));
    }
    for(int x = 0;x < width;++x){
        for(int y = 0;y < height;++y){
            if(roadData[x+y*width] != 0 && fastestWayOut[x+y*width] != 'O'){
                /*sf::Color col(0,0,0);
                if(fastestWayOut[x+y*width] == 'u'){
                    col = sf::Color::Green;
                }
                if(fastestWayOut[x+y*width] == 'd'){
                    col = sf::Color::Red;
                }
                if(fastestWayOut[x+y*width] == 'l'){
                    col = sf::Color::Blue;
                }
                if(fastestWayOut[x+y*width] == 'r'){
                    col = sf::Color::Yellow;
                }
                r.Draw(sf::Shape::Rectangle(x*gridSize-roadWidth,y*gridSize-roadWidth,x*gridSize+roadWidth,y*gridSize+roadWidth, col));*/
                if((roadData[x+y*width] & downRoad) == downRoadDown){
                    r.Draw(sf::Shape::Line(x*gridSize + arrowSpread, (y+lowArrow)*gridSize, x*gridSize, (y+highArrow)*gridSize, 1, sf::Color::Black));
                    r.Draw(sf::Shape::Line(x*gridSize - arrowSpread, (y+lowArrow)*gridSize, x*gridSize, (y+highArrow)*gridSize, 1, sf::Color::Black));
                }
                if((roadData[x+y*width] & downRoad) == downRoadUp){
                    r.Draw(sf::Shape::Line(x*gridSize + arrowSpread, (y+highArrow)*gridSize, x*gridSize, (y+lowArrow)*gridSize, 1, sf::Color::Black));
                    r.Draw(sf::Shape::Line(x*gridSize - arrowSpread, (y+highArrow)*gridSize, x*gridSize, (y+lowArrow)*gridSize, 1, sf::Color::Black));
                }
                if((roadData[x+y*width] & rightRoad) == rightRoadRight){
                    r.Draw(sf::Shape::Line((x+lowArrow)*gridSize,y*gridSize + arrowSpread, (x+highArrow)*gridSize, y*gridSize, 1, sf::Color::Black));
                    r.Draw(sf::Shape::Line((x+lowArrow)*gridSize,y*gridSize - arrowSpread, (x+highArrow)*gridSize, y*gridSize, 1, sf::Color::Black));
                }
                if((roadData[x+y*width] & rightRoad) == rightRoadLeft){
                    r.Draw(sf::Shape::Line((x+highArrow)*gridSize,y*gridSize + arrowSpread, (x+lowArrow)*gridSize, y*gridSize, 1, sf::Color::Black));
                    r.Draw(sf::Shape::Line((x+highArrow)*gridSize,y*gridSize - arrowSpread, (x+lowArrow)*gridSize, y*gridSize, 1, sf::Color::Black));
                }
            }
        }
    }
    sf::Sprite carspr;
    carspr.SetImage(carimg);
    carspr.SetCenter(4, 4);
    for(std::list<car>::iterator i = cars.begin();i != cars.end();++i){
        /*r.Draw(sf::Shape::Circle(i->getX() * gridSize,  i->getY() * gridSize, carRadius, sf::Color(0,0,0,(i->transition == 'O')?(1-i->transitionProgress)*255:255)));*/
        carspr.SetColor(i->getColor());
        if(i->transition == 'O' || i->transition == 'l'){
            carspr.SetRotation(0);
        }else if(i->transition == 'u'){
            carspr.SetRotation(90);
        }else if(i->transition =='r'){
            carspr.SetRotation(180);
        }else if(i->transition == 'd'){
            carspr.SetRotation(270);
        }
        carspr.SetPosition(i->getX() * gridSize, i->getY()*gridSize);
        r.Draw(carspr);
    }
    r.Draw(sf::Shape::Rectangle(deadlyWave*gridSize, 0, width*gridSize, height*gridSize, sf::Color(0,0,255,128)));
}
bool levelData::routeExists(int x, int y, char dir){
    if((x <= 0 && dir != 'r') || (y <= 0 && dir !='d') || (x >= width && dir != 'l') || (y >= height && dir !='u')) return false;
    if(dir == 'u'){
        return (roadData[POS(x,y-1)] & levelData::downRoadDown) != 0;
    }
    if(dir == 'd'){
        return (roadData[POS(x,y)] & levelData::downRoadUp) != 0;
    }
    if(dir == 'l'){
        return (roadData[POS(x-1,y)] & levelData::rightRoadRight) != 0;
    }
    if(dir == 'r'){
        return (roadData[POS(x,y)] & levelData::rightRoadLeft) != 0;
    }
}
bool levelData::hasRoad(int x, int y, char dir){
    if(dir == 'a') return hasRoad(x, y, 'v') || hasRoad(x, y, 'h');
    if(dir == 'v') return hasRoad(x, y, 'u') || hasRoad(x, y, 'd');
    if(dir == 'h') return hasRoad(x, y, 'r') || hasRoad(x, y, 'l');
    if(x <= 0 || y <= 0 || x >= width || y >= height) return false;
    if(dir == 'u'){
        return (roadData[POS(x,y-1)] & levelData::downRoad) != 0;
    }
    if(dir == 'd'){
        return (roadData[POS(x,y)] & levelData::downRoad) != 0;
    }
    if(dir == 'l'){
        return (roadData[POS(x-1,y)] & levelData::rightRoad) != 0;
    }
    if(dir == 'r'){
        return (roadData[POS(x,y)] & levelData::rightRoad) != 0;
    }
}
#include <list>
#define addToWorst(x,y,d) worstBack->next = new fastestList(x,y);\
    worstBack = worstBack->next;\
    fastestWayOut[(x)+(y)*width] = d
class fastestList{
public:
    int x,y;
    fastestList* next;
    fastestList(int x,int y,fastestList* n = 0):x(x),y(y),next(n){}
};
void levelData::generateFastest(){
    bool* open = new bool[width*height];
    fastestList* worstFront = 0;
    fastestList* worstBack = 0;
    for(int i = 0;i < width*height;++i) open[i] = true;
    for(int x = 0;x < width;++x){
        //worst.push_back(std::make_pair(x, 0));
        //worst.push_back(std::make_pair(x, height));
        open[x] = false;
    }
    for(int y = 0;y < height;++y){
        worstFront = new fastestList(0,y,worstFront);
        if(worstBack == 0) worstBack = worstFront;
        //worst.push_back(std::make_pair(width, y));
        open[y*width] = false;
    }
    unsigned int maxAccess = width*height;
    while(worstFront != 0){
        worstBack->next = new fastestList(-1,-1);
        worstBack = worstBack->next;
        while(worstFront->x != -1){
            std::pair<int,int> i = std::make_pair(worstFront->x, worstFront->y);
            int p = i.first+i.second*width;
            if(p-width >= 0 && routeExists(i.first, i.second, 'u') && open[p - width]){
                addToWorst(i.first, i.second-1, 'd');
                open[p-width] = false;
            }
            if(p+width < maxAccess && routeExists(i.first, i.second, 'd') && open[p + width]){
                addToWorst(i.first, i.second+1, 'u');
                open[p+width] = false;
            }
            if(p-1 >= 0 && routeExists(i.first, i.second, 'l') && open[p - 1]){
                addToWorst(i.first-1, i.second, 'r');
                open[p-1] = false;
            }
            if(p-1 < maxAccess && routeExists(i.first, i.second, 'r') && open[p + 1]){
                addToWorst(i.first+1, i.second, 'l');
                open[p+1] = false;
            }
            fastestList* f = worstFront;
            worstFront = worstFront->next;
            delete f;
        }
        fastestList* f = worstFront;
        worstFront = worstFront->next;
        delete f;
    }
}
roadRow levelData::fromPoint(int x, int y, char dir){
    roadRow ret;
    ret.x1 = x;
    ret.x2 = x;
    ret.y1 = y;
    ret.y2 = y;
    if(dir == 'v'){
        ++ret.y2;
        while(hasRoad(x, ret.y1, 'u') && !hasRoad(x, ret.y1, 'h')){
            --ret.y1;
        }
        while(hasRoad(x, ret.y2, 'd') && !hasRoad(x, ret.y2, 'h')){
            ++ret.y2;
        }
    }
    if(dir == 'h'){
        ++ret.x2;
        while(hasRoad(ret.x1, y, 'l') && !hasRoad(ret.x1, y, 'v')){
            --ret.x1;
        }
        while(hasRoad(ret.x2, y, 'r') && !hasRoad(ret.x2, y, 'v')){
            ++ret.x2;
        }
    }
    return ret;
}
#include <sstream>
inline float max0(float a){
    if(a < 0) return 0;
    return a;
}
void levelData::drawHover(sf::RenderWindow& r,int mX, int mY){
    sf::String Text;
    std::stringstream s;
    s << "Rescued " << escaped << " Cars";
    Text.SetText(s.str());
    Text.SetSize(20);
    Text.SetColor(sf::Color(255,255,255));
    sf::FloatRect rz = Text.GetRect();
    Text.SetPosition(10, 517);
    r.Draw(sf::Shape::Rectangle(0, 512, 640, 562, sf::Color(0,0,0)));
    r.Draw(Text);
    s.str("");
    s << alive << " Cars Remaining";
    Text.SetText(s.str());
    rz = Text.GetRect();
    Text.SetPosition(630-rz.GetWidth(), 517);
    r.Draw(Text);
    int xBelow = (mX + roadWidth) / gridSize;
    int yBelow = (mY + roadWidth) / gridSize;
    bool mXRoadPos = ((mX + roadWidth) % gridSize <= 2*roadWidth);
    bool mYRoadPos = ((mY + roadWidth) % gridSize <= 2*roadWidth);
    if(mXRoadPos && mYRoadPos){
        mXRoadPos = hasRoad(xBelow, yBelow, 'v');
        mYRoadPos = hasRoad(xBelow, yBelow, 'h');
        if(mXRoadPos && mYRoadPos) return;
    }
    if(mYRoadPos && ((roadData[xBelow+yBelow*width] & levelData::rightRoad) != 0)){
        roadRow q = fromPoint(xBelow, yBelow, 'h');
        r.Draw(sf::Shape::Rectangle(q.x1*gridSize + roadWidth, q.y1*gridSize + roadWidth, q.x2*gridSize - roadWidth, q.y2*gridSize - roadWidth, sf::Color(255,255,0,128)));
    }else if(mXRoadPos && ((roadData[xBelow+yBelow*width] & levelData::downRoad) != 0)){
        roadRow q = fromPoint(xBelow, yBelow, 'v');
        r.Draw(sf::Shape::Rectangle(q.x1*gridSize + roadWidth, q.y1*gridSize + roadWidth, q.x2*gridSize - roadWidth, q.y2*gridSize - roadWidth, sf::Color(255,255,0,128)));
    }else{
        for(unsigned int i = 0;i < buildings.size();++i){
            if(xBelow >= buildings[i].x && xBelow < buildings[i].x + buildings[i].width && yBelow >= buildings[i].y && yBelow < buildings[i].y + buildings[i].height){
                r.Draw(sf::Shape::Rectangle(buildings[i].x*gridSize+roadWidth, buildings[i].y*gridSize+roadWidth, (buildings[i].x + buildings[i].width)*gridSize-roadWidth, (buildings[i].y + buildings[i].height)*gridSize-roadWidth, sf::Color(255,255,0,128)));
                return;
            }
        }
        return;
    }
}
void levelData::handleClick(int mX, int mY){
    int xBelow = (mX + roadWidth) / gridSize;
    int yBelow = (mY + roadWidth) / gridSize;
    bool mXRoadPos = ((mX + roadWidth) % gridSize <= 2*roadWidth);
    bool mYRoadPos = ((mY + roadWidth) % gridSize <= 2*roadWidth);
    if(mXRoadPos && mYRoadPos){
        mXRoadPos = hasRoad(xBelow, yBelow, 'v');
        mYRoadPos = hasRoad(xBelow, yBelow, 'h');
        if(mXRoadPos && mYRoadPos) return;
    }
    if(mYRoadPos && ((roadData[xBelow+yBelow*width] & levelData::rightRoad) != 0)){
        roadRow q = fromPoint(xBelow, yBelow, 'h');
        if(q.x1 == 0 || q.x2 == width) return;
        int mode = roadData[xBelow+yBelow*width] & levelData::rightRoad;
        if(mode == levelData::rightRoadRight){
            mode = levelData::rightRoadLeft;
        }else if(mode == levelData::rightRoadLeft){
            mode = levelData::rightRoad;
        }else{
            mode = rightRoadRight;
        }
        for(int x = q.x1;x < q.x2;++x){
            roadData[x+yBelow*width] = roadData[x+yBelow*width] & ~levelData::rightRoad | mode;
        }
    }else if(mXRoadPos && ((roadData[xBelow+yBelow*width] & levelData::downRoad) != 0)){
        roadRow q = fromPoint(xBelow, yBelow, 'v');
        if(q.y1 == 0 || q.y2 == height) return;
        int mode = roadData[xBelow+yBelow*width] & levelData::downRoad;
        if(mode == levelData::downRoadDown){
            mode = levelData::downRoadUp;
        }else if(mode == levelData::downRoadUp){
            mode = levelData::downRoad;
        }else{
            mode = levelData::downRoadDown;
        }
        for(int y = q.y1;y < q.y2;++y){
            roadData[xBelow+y*width] = roadData[xBelow+y*width] & ~levelData::downRoad | mode;
        }
    }else{
        for(unsigned int i = 0;i < buildings.size();++i){
            if(buildings[i].spawnX > deadlyWave){
                buildings[i].pop = 0; //They all drowned.
            }
            if(xBelow >= buildings[i].x && xBelow < buildings[i].x + buildings[i].width && yBelow >= buildings[i].y && yBelow < buildings[i].y + buildings[i].height){
                buildings[i].spawning = !buildings[i].spawning;
                return;
            }
        }
        return;
    }
    generateFastest();
}
#define updateFloat 0.025
sf::Color car::getColor(){
    if(transition == 'O'){
        return sf::Color(r,g,b,(1-transitionProgress)*255);
    }
    return sf::Color(r,g,b,255);
}
float car::getX(){
    if(transition == 'l'){
        return x + transitionProgress;
    }
    if(transition == 'r'){
        return x - transitionProgress;
    }
    return x;
}
float car::getY(){
    if(transition == 'u'){
        return y + transitionProgress;
    }
    if(transition == 'd'){
        return y - transitionProgress;
    }
    return y;
}
bool levelData::update(){
    deadlyWave -= deadlyWaveSpeed;
    for(std::list<car>::iterator i = cars.begin();i != cars.end();++i){
        if(i->transitionProgress > updateFloat) i->transitionProgress -= updateFloat;
        else{
            if(i->x > deadlyWave){
                roadData[i->x + i->y*width] = roadData[i->x + i->y*width] & ~occupied;
                cars.erase(i--);
                --alive;
                ++carsLostToWave;
                continue;
            }
            if(i->x == 0 || i->y == 0 || i->x == width || i->y == height){
                cars.erase(i--);
                ++escaped;
                --alive;
                continue;
            }
            roadData[i->x + i->y*width] = roadData[i->x + i->y*width] & ~occupied;
            i->transition = fastestWayOut[i->x + i->y*width];
            i->transitionProgress = 1;
            int oldX = i->x;
            int oldY = i->y;
            if(i->transition == 'u'){
                --i->y;
            }
            if(i->transition == 'd'){
                ++i->y;
            }
            if(i->transition == 'l'){
                --i->x;
            }
            if(i->transition == 'r'){
                ++i->x;
            }
            if(i->x != width && i->y != height && i->x != 0 && i->y != 0 && (roadData[i->x + i->y*width] & occupied) != 0){
                i->x = oldX;
                i->y = oldY;
                i->transitionProgress = 0;
            }
            if(i->x != width && i->y != height && i->x != 0 && i->y != 0) roadData[i->x + i->y*width] = roadData[i->x + i->y*width] | occupied;
        }
    }
    for(unsigned int i = 0;i < buildings.size();++i){
        if(deadlyWave < buildings[i].spawnX){
            alive -= buildings[i].pop;
            buildings[i].pop = 0; //Murder everyone.
        }
        if(buildings[i].spawning && buildings[i].pop > 0 && (roadData[buildings[i].spawnX + buildings[i].spawnY*width] & occupied) == 0){
            cars.push_back(car(buildings[i].spawnX,buildings[i].spawnY,'O',1));
            _addOption(roadData[buildings[i].spawnX + buildings[i].spawnY*width],occupied);
            if(--buildings[i].pop == 0){
                ++buildingsEvacuated;
            }
        }
    }
    return alive == 0;
}