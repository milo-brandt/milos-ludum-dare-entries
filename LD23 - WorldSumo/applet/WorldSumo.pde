import processing.opengl.PGraphicsOpenGL;
import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;
Player x;
Input in;
Resources r;
ArrayList<Planet> planets;
ArrayList<PlanetBoom> planetBooms;
float lastFree = HALF_PI; //We're using (sin(x),cos(x))
float timeSince;
float totalTime;
int frames = 60;
float leadTime = 5*frames;
float cullDist;
float scaleF = 0.5;
float maxDist = 3000;
float score;
float maxChange = 150*scaleF;
float killThreshold = 1;
float deathProgress = 0;
float transitionIn;
int mode = 0;
Minim minim;
AudioPlayer music;
AudioSnippet jump;
void setup() {
  println("Going");
  size(640,480,OPENGL);
  noStroke();
  frameRate(frames);
  cullDist = sqrt(sq(width) + sq(height))/2;
  in = new Input();
  r = new Resources();
  minim = new Minim(this);
  music = minim.loadFile("Theme.mp3");
  jump = minim.loadSnippet("jump.wav");
  music.play();
  music.loop();
  music.setGain(-5);
}
void stop()
{
  music.close();
  jump.close();
  minim.stop();
  super.stop();
}
void startGame(){
  transitionIn = 1;
  timeSince = 0;
  totalTime = 0;
  deathProgress = 0;
  mode = 1;
  planetBooms = new ArrayList<PlanetBoom>();
  planets = new ArrayList<Planet>();
  planets.add(new Planet(scaleF*150,new Vector2(0,scaleF*150+10),0,0,new Vector2(0,0),10*frames));
  x = new Player(new Vector2(0,0),planets.get(0),10,4.5*scaleF);
  score = 0;
}
boolean drawButton(float x,float y,float halfw,float halfh){
  boolean isOver = abs(mouseX-x) < halfw && abs(mouseY-y) < halfh;
  fill(128,128,isOver?0:128);
  rect(x-halfw,y-halfh,halfw*2,halfh*2);
  fill(200,200,isOver?0:200);
  rect(x-halfw+5,y-halfh+5,halfw*2-10,halfh*2-10);
  fill(255);
  return isOver;
}
void drawTitle(){
  textAlign(CENTER,CENTER);
  textFont(r.score,40);
  text("Interplanetary Sumo",width/2,50);  
  textFont(r.score,30);
  if(drawButton(width/2,150,60,25) && mousePressed && deathProgress == 0){
    deathProgress = 1/120.0;
  }
  fill(0);
  text("Play!",width/2,150);
  fill(255);
  textFont(r.score,20);
  textAlign(LEFT,TOP);
  text("Instructions: Arrows keys (or WASD) to move. Space\n(or W or Up) to jump. Stay near the center of the\nfield. The meter in the top right displays your\ndistance from the center.",20,250);
}
float boxSize = 30;
void stars(){ 
 int rseed = int(random(0,1000000));
 Vector2 cent = Vector2.sum(x.screenBounds.c,new Vector2(-cullDist,-cullDist));;
 Vector2 start = new Vector2(floor(cent.x/boxSize)*boxSize,floor(cent.y/boxSize)*boxSize);
 for(int i = 0;i <= ceil(cullDist*2/boxSize);++i){
   for(int j = 0;j <= ceil(cullDist*2/boxSize);++j){
     Vector2 corn = new Vector2(start.x + i*boxSize,start.y + j*boxSize);
     randomSeed(int(corn.x*999 + corn.y*9999));
     if(random(1) > 0.7){
     fill(random(1)*255);
       //ellipse(corn.x,corn.y,2,2);
       ellipse(corn.x + random(0,boxSize),corn.y + random(0,boxSize),2,2);
     }
     
   }
 }
 randomSeed(rseed);
 fill(255);
}
void drawArrow(){ //Dear DRY: Sorry
   Vector2 transform = x.screenBounds.transform(new Vector2(0,0));
     if(abs(transform.x) < width/2 && abs(transform.y) < height/2){
       return;
     }
     Vector2 veloc = x.screenBounds.transformSpeed(x.pos);
     float dx = ((transform.x > 0?1:-1)*width/2 - transform.x)/veloc.x;
     float dy = ((transform.y > 0?1:-1)*height/2 - transform.y)/veloc.y;
     Vector2 xInt = Vector2.sum(transform,veloc.mul(dx));
     Vector2 yInt = Vector2.sum(transform,veloc.mul(dy));
     Vector2 inter;
     if(abs(yInt.x) > width/2){
       inter = xInt;
     }else if(abs(xInt.y) > height/2){
       inter = yInt;
     }else{
       inter = dx<dy?xInt:yInt;
     }
     fill(255,255,0);
     drawSideBox(inter,5,10);
     fill(255);
}
void drawDistometer(){
  float safety = (maxDist-x.pos.magnitude())/maxDist;
  float barheight = 100;
  noStroke();
  fill(255,255,0);
  rect(width-30,10+(1-safety)*barheight,20,safety*barheight);
  noFill();
  stroke(255);
  strokeWeight(2);
  rect(width-30,10,20,barheight);
  noStroke();
  fill(255);
}
float getMinSpeed(){
  if(score < 1000) return 2;
  if(score < 3500) return 3;
  if(score < 7000) return 5;
  return 8;
}
float getSpawnRate(){
  if(score < 1000) return 90;
  if(score < 3500) return 60;
  if(score < 7000) return 40;
  return 30;
}
boolean isHuge(){
  if(score < 1000) return false;
  if(score < 3500) return random(1) > 0.975;
  if(score < 7000) return random(1) > 0.95;
  return random(1) > 0.9;
}
boolean isSpeedy(){
  if(score < 1000) return false;
  if(score < 3500) return random(1) > 0.975;
  if(score < 7000) return random(1) > 0.95;
  return random(1) > 0.9; 
}
//On average, every 50 seconds when score >= 7000, a huge, speedy planet will spawn
void spawnFree(){
  Vector2 target = new Vector2(x.home.pos.x + random(-200,200),x.home.pos.y + random(-200,200));
  lastFree += random(HALF_PI,3*HALF_PI);
  if(lastFree > TWO_PI) lastFree -= TWO_PI;
  float speed = (isSpeedy()?scaleF*random(1,2):scaleF*2.5)*getMinSpeed();
  float timeAway = 10;
  float radi = isHuge()?scaleF*300:scaleF*random(50,150);
  float distAway = 2*cullDist + radi;
  Vector2 pos = Vector2.angleRadius(lastFree,distAway);
  planets.add(new Planet(radi,Vector2.sum(pos,target),0,random(-0.05,0.05),Vector2.sum(x.home.veloc,Vector2.angleRadius(lastFree,-speed)),timeAway*frames+totalTime));
}
void drawSideBox(Vector2 pos,float rad,float h){ //Yeah, this function is mostly polish.
  if(abs(abs(pos.x) - width/2) < 1.0){
    boolean neg = pos.x < 0;
    float edgeDist = height/2-abs(pos.y);
    rect(neg?0:(width-h),pos.y-rad+height/2,h,2*rad);
    if(edgeDist < rad){
      boolean neg2 = pos.y < 0;
      float wrap = sqrt(sq(rad) - sq(edgeDist));
      if(wrap > h){
        rect(neg?h:(width-wrap),neg2?0:(height-h),wrap-h,h);
      }
    }
  }else{
    boolean neg = pos.y < 0;
    float edgeDist = width/2-abs(pos.x);
    rect(pos.x-rad+width/2,neg?0:(height-h),2*rad,h);
    if(edgeDist < rad){
      boolean neg2 = pos.x < 0;
      float wrap = sqrt(sq(rad) - sq(edgeDist));
      if(wrap > h){
        rect(neg2?0:(width-h),neg?h:(height-wrap),h,wrap-h);
      }
    }
  }
}
/*void drawWarnings(){ 
 for(int i = 0;i < planets.size();++i){
   Planet b = planets.get(i);
   if(b.expectedImpact < totalTime + leadTime){
     Vector2 transform = x.screenBounds.transform(b.pos);
     if(abs(transform.x) < width/2 && abs(transform.y) < height/2){
       continue;
     }
     Vector2 veloc = x.screenBounds.transformSpeed(b.veloc);
     float dx = ((transform.x > 0?1:-1)*width/2 - transform.x)/veloc.x;
     float dy = ((transform.y > 0?1:-1)*height/2 - transform.y)/veloc.y;
     Vector2 xInt = Vector2.sum(transform,veloc.mul(dx));
     Vector2 yInt = Vector2.sum(transform,veloc.mul(dy));
     Vector2 inter;
     if(abs(yInt.x) > width/2){
       inter = xInt;
     }else if(abs(xInt.y) > height/2){
       inter = yInt;
     }else{
       inter = dx<dy?xInt:yInt;
     }
     fill(255,0,0,255*(1-sq((b.expectedImpact - totalTime)/leadTime)));
     drawSideBox(inter,b.maxRadius,10);
   }
 }
 fill(255);
}*/
void physics(float dt){
  totalTime += dt;
  timeSince += dt;
  float minDist = maxChange;
  Planet minOn = null;
  boolean canSquish = x.on != null && Vector2.distance(x.pos,x.on.pos) < x.on.maxRadius + x.playerRadius + 1;
  if(timeSince > getSpawnRate()){
    timeSince = 0;
    spawnFree();
  }
  //x.on.health = min(120,x.on.health+2);
  for(int i = 0;i < planets.size();++i){
    planets.get(i).update();
  }
  for(int i = 0;i < planets.size();++i){
    Planet b = planets.get(i);
    for(int j = i + 1; j < planets.size();++j){
      Planet c = planets.get(j);
      if(b.collideWith(c) && b == x.on){
        //Vector2 pos = Vector2.sum(b.pos,Vector2.dif(c.pos,b.pos).unit(b.maxRadius));
      }
    }
    //b.health -= 1;
    if(x.collideWith(b)){
      if(b != x.on){
        if(canSquish && x.on != null && Vector2.distance(x.pos,x.on.pos) < x.on.maxRadius + x.playerRadius - 1){
          x.kill();
        }
        in.controlsDown.remove(Input.JUMP);
        x.switchTo(b);
      }
    }
    float pdist = Vector2.distance(x.pos,b.pos) - b.maxRadius;
    if(pdist < minDist){
      minDist = pdist;
      minOn = b;
    }
    if(b.health <= 0 || b.pos.magnitude() > maxDist + 1.5*cullDist){
      if(b == x.on){
        x.on = null;
        x.veloc = Vector2.sum(Vector2.dif(x.home.pos,x.pos).unit(x.downSpeed),x.veloc);
        x.downSpeed = 0;
      }
      planets.remove(i);
      --i;
      if(b.health <= 0) planetBooms.add(new PlanetBoom(b));
    }
  }
  if(minOn != null){
    in.controlsDown.remove(Input.JUMP);
    x.switchTo(minOn);
  }
}
void draw() {
  background(0);
  if(mode == 1){
  x.setCamera();
  stars();
  for(int i = 0;i < planets.size();++i){
    planets.get(i).render(r);  
  }
    for(int i = 0;i < planetBooms.size();++i){
    if(planetBooms.get(i).render()){
      planetBooms.remove(i);
      --i;
    }  
  }
  if(x.alive){
  x.movePlyr(in);
  physics(1);
  }
  ellipse(0,0,10,10);
  float safety = (maxDist-x.pos.magnitude())/maxDist;
  if(x.alive) score += sq(safety);
  stroke(255,safety*255,safety*255,(1-safety)*128+32);
  strokeWeight(4);
  line(x.pos.x,x.pos.y,0,0);
  noFill();
  stroke(255,255,255);
  ellipse(0,0,maxDist*2,maxDist*2);
  fill(255);
  noStroke();
  camera();
  //drawWarnings();
  drawArrow();
  drawDistometer();
    if(!x.alive){
    deathProgress = min(1,deathProgress + 1/120.0);
    fill(0,0,0,deathProgress*255);
    rect(0,0,width,height);
    fill(255);
        if(deathProgress == 1){
      mode = 2;
      deathProgress = 0;
      transitionIn = 1;
    }
  }  
  textAlign(CENTER,CENTER);
  textFont(r.score,20 + deathProgress*40);
  text("Score: " + int(score),width/2,30*(1-deathProgress)+height/2*deathProgress);
    if(transitionIn != 0){
    transitionIn = max(0,transitionIn - 1/120.0);
    fill(0,0,0,transitionIn*255);
    rect(0,0,width,height);
    fill(255);
  }
}
  if(mode == 0){
    drawTitle();
           if(transitionIn != 0){
      transitionIn = max(0,transitionIn - 1/60.0);
      fill(0,0,0,transitionIn*255);
      rect(0,0,width,height);
           fill(255);

    }    if(deathProgress != 0){
      deathProgress = min(1,deathProgress + 1/60.0);
      fill(0,0,0,deathProgress*255);
      rect(0,0,width,height);
      fill(255);
    }
    if(deathProgress == 1) startGame();
  }
  if(mode == 2){
    if(drawButton(width/2,height/2+100,60,25) && mousePressed && deathProgress == 0){
      deathProgress = 1/60.0;
    }
    fill(0);
    textAlign(CENTER,CENTER);
    textFont(r.score,30);
    text("Back",width/2,height/2+100);
     fill(255);
                    if(transitionIn != 0){
      transitionIn = max(0,transitionIn - 1/60.0);
      fill(0,0,0,transitionIn*255);
      rect(0,0,width,height);
           fill(255);

    }

    textFont(r.score,60);
    text("Score: " + int(score),width/2,height/2);
       if(deathProgress != 0){
      deathProgress = min(1,deathProgress + 1/60.0);
      fill(0,0,0,deathProgress*255);
      rect(0,0,width,height);
           fill(255);

    }
        if(deathProgress == 1){
      transitionIn = 1;
      deathProgress = 0;
      mode = 0;
    }
  }
}
void keyPressed(){
  Object ctrl;
  if(key == 'k'){
    saveFrame("screen-####.png");
  }
  if(key == CODED){
    ctrl = in.keyCodesToControls.get(keyCode);
  }else{
    ctrl = in.keysToControls.get(key);
  }
  if(ctrl != null){
    in.controlsDown.add(ctrl);
  }
}
void keyReleased(){
  Object ctrl;
  if(key == CODED){
    ctrl = in.keyCodesToControls.get(keyCode);
  }else{
    ctrl = in.keysToControls.get(key);
  }
  if(ctrl != null){
    in.controlsDown.remove(ctrl);
  }
}
