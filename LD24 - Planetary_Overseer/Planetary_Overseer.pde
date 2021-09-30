import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;

float controlsSize = 64;
float lostFade = 1;
Minim minim;
AudioPlayer music;
class vector2{
  public float x;
  public float y;
  vector2(float x,float y){
    this.x = x;
    this.y = y;
  }
  float magnitude(){
    return sqrt(x*x + y*y);
  }
  float magnitudeSq(){
    return x*x + y*y;
  }
  vector2 unit(float r){
    if(x == 0 && y == 0) return this;
    float m = r/magnitude();
    return new vector2(x*m,y*m);
  }
  vector2 unit(){
    return unit(1);
  }
  vector2 subt(vector2 b){
    return new vector2(x-b.x,y-b.y);
  }
  vector2 addt(vector2 b){
    return new vector2(x+b.x,y+b.y);
  }
  vector2 mul(float f){
    return new vector2(f*x,f*y);
  }
  vector2 clip(){
    return new vector2(max(min(x,width),0),max(min(y,height-controlsSize),0));
  }
}
vector2 newV(float x,float y){
  return new vector2(x,y);
}
organism getClosest(ArrayList<organism> orgs,vector2 p,int exclude,int exclude2){
  float curBest = orgs.get(0).pos.subt(p).magnitudeSq() - sq(orgs.get(0).girth);
  organism cur = orgs.get(0);
  if(cur.species == exclude || cur.species == exclude2) curBest = 1000000000000000.f;
  for(int i = 1;i < orgs.size();++i){
    if(orgs.get(i).species == exclude || orgs.get(i).species == exclude2) continue;
    if(orgs.get(i).girth <= 0) continue;
    float next = orgs.get(i).pos.subt(p).magnitudeSq() - sq(orgs.get(i).girth);
    if(next == 0) continue; //Probably yourself. Ignore.
    if(next < curBest){
      curBest = next;
      cur = orgs.get(i);
    }
  }
  return cur;
}
vector2 randomV(){
  return newV(random(0,width),random(0,height-controlsSize));
}
vector2 randomV(vector2 c,float r){ //You know, the Chevychev metric is the same as the Euclidean one...
  float ang = random(0,TWO_PI);
  return newV(c.x + cos(ang)*r,c.y + sin(ang)*r).clip();
}
class organism{
  public int species; //Really, species hardly matters...
  public vector2 pos;
  public vector2 target;
  public organism kill;
  public float girth;
  public float strength;
  public float speed;
  public float lastUpdate;
  public boolean hasEaten;
  public float foodLevel;
  public float photosynthesis; 
  public float starve;
  public float juvenilePause;
  public float birthPause;
  public float birthRange; //Projectile babies!
  public float birthSpeed;
  public float girthGoal;
  public float killPauseTime;
  public boolean herbivore;
  public float eggTime;
  organism(organism b,vector2 pos){
    speciesCount[b.species]++;
    this.species = b.species;
    this.pos = pos;
    this.target = pos;
    this.girth = max(b.girth + random(1,-1),4);
    this.speed = b.speed;
    this.lastUpdate = 0;
    this.foodLevel = 0;
    this.hasEaten = false;
    this.juvenilePause = b.eggTime;
    this.birthPause = b.birthSpeed;
    this.photosynthesis = b.photosynthesis;
    this.starve = b.starve;
    this.birthRange = b.birthRange;
    this.strength = b.strength;
    this.girthGoal = b.girthGoal;
    this.herbivore = b.herbivore;
    this.killPauseTime = b.killPauseTime;
    this.birthSpeed = b.birthSpeed;
    this.eggTime = b.eggTime;
  }
  organism(int species,vector2 pos,vector2 target,float girth,float strength,float speed,float photosynthesis,float starve,float birthRange,float birthSpeed,float eggTime,boolean herbivore,float killPauseTime){
    this.species = species;
    this.pos = pos;
    this.target = target;
    this.girth = girth;
    this.strength = strength;
    this.speed = speed;
    this.lastUpdate = 0;
    this.foodLevel = 0;
    this.hasEaten = false;
    this.photosynthesis = photosynthesis;
    this.starve = starve;
    this.juvenilePause = 0;
    this.birthPause = 0;
    this.birthRange = birthRange;
    this.girthGoal = girth;
    this.herbivore = herbivore;
    this.killPauseTime = killPauseTime;
    this.birthSpeed = birthSpeed;
    this.eggTime = eggTime;
  }
  boolean willFight(organism b){
    return winP(b) > 0.35;
  }
  float getCombat(){
    return strength*girth;
  }
  float winP(organism b){
    float girthRatio = getCombat()/b.getCombat();
    float worstGirth = 1.5;
    if(girthRatio >= worstGirth) return 1;
    if(girthRatio <= 1/worstGirth) return 0;
    return (1+log(girthRatio)/log(worstGirth))/2;
  }
  float distTo(organism b){
    float raw = pos.subt(b.pos).magnitudeSq(); //(act+girth)^2
    return sqrt(raw) - girth - b.girth; 
  }
  void lookAround(){
    organism pfriendly = getClosest(animals,pos,-1,herbivore?-1:0);
    organism enemy;
    if(pfriendly.species != species){
      enemy = pfriendly;
    }else{
      enemy = getClosest(animals,pos,species,herbivore?-1:0);
    }
    if(enemy.species != species && distTo(enemy) < 400 && (!herbivore || pfriendly.species != 0)){
      if(kill == null || kill.girth == 0 || distTo(kill) > 2*distTo(enemy)){
      kill = enemy;
      }
    }else{
      kill = null;
      if(target.subt(pos).magnitudeSq() < 100){
      if(distTo(pfriendly) < 32){
        target = randomV();
      }else if(distTo(pfriendly) < 400){
        target = randomV();//pfriendly.target,16);
      }
      }
    }
  }
  boolean fleeing(){
    return kill != null && !willFight(kill);
  }
  void execute(organism b){
    if(!herbivore && b.species == 0) return;
    if(juvenilePause > 0) return;
    if(foodLevel > 5) return;
    b.kill = this;
    girth += b.girth/girth;
    foodLevel += b.girth/2;
    if(foodLevel > 6) foodLevel = 6;
    b.girth = 0;
    totalPhoto -= b.photosynthesis;
    kill = null;
    lastUpdate = 0;
    juvenilePause = killPauseTime;
  }
  void update(float dT){
    if(juvenilePause > 0){
      juvenilePause -= dT;
      return;
    }
    if(birthPause > 0){
      birthPause -= dT;
    }
    foodLevel += dT*(photosynthesis - starve*girth);
    
    float moveD = dT*speed;
    if(kill != null){
      target = kill.pos;
      if(!willFight(kill)){
        moveD = -moveD; //Run!
      }
      
      if(distTo(kill) < 10){
        if(random(0,1) < winP(kill)){
          execute(kill);
        }else{
          kill.execute(this);
        }
      }
    }
    lastUpdate -= dT;
    vector2 dif = target.subt(pos);
    if(lastUpdate <= 0 || (kill == null && dif.magnitudeSq() < sq(moveD))){
      lookAround();
      lastUpdate = 1;
      dif = target.subt(pos);
    }
    pos = pos.addt(dif.unit(moveD)).clip();
    if(moveD < 0 && abs(pos.x - width/2) + abs(pos.y - (height-controlsSize)/2) > (width/2 + (height-controlsSize)/2)-50){
      dif = newV(width/2,(height-controlsSize)/2).subt(pos);
      pos = pos.addt(dif.unit(moveD*-2)).clip();
    }
  }
}
class particleRing{
  vector2[] positions;
  vector2[] velocities;
  float maxLife;
  float nowLife;
  int num;
  particleRing(int num,vector2 startC,float startRad,float speed,float life){
    this.num = num;
    this.maxLife = life;
    this.nowLife = life;
    positions = new vector2[num];
    velocities = new vector2[num];
    for(int i = 0;i < num;++i){
      positions[i] = randomV(startC,sqrt(random(0,1))*startRad);
      float ang = random(0,TWO_PI);
      float rad = sqrt(random(0.1,1))*speed;
      velocities[i] = newV(cos(ang)*rad,sin(ang)*rad);
    }
  }
  void updateDraw(float dT){
    nowLife -= dT;
    if(nowLife < 0) return;
    fill(128,128,128,255*nowLife/maxLife);
    for(int i = 0;i < num;++i){
      println("YAY" + positions[i].x);
      positions[i] = positions[i].addt(velocities[i].mul(dT));
      ellipse(positions[i].x,positions[i].y,25,25);
    }
  }
}
class color3{
  public float r;
  public float g;
  public float b;
  color3(float r,float g,float b){
    this.r = r;
    this.g = g;
    this.b = b;
  }
  void setStroke(){
    stroke(r,g,b);
  }
  void setFill(){
    fill(r,g,b);
  }
}
ArrayList<color3> specieColor;
ArrayList<vector2> specieSpawns;
ArrayList<organism> animals;
ArrayList<organism> children;
ArrayList<organism> dying;
float totalPhoto = 0;
float lastM = 0;
PImage herbivore;
PImage algae;
PImage predator;
PImage[] powerUps;
int[] powerup;
String[] powdesc;
float powerupcooldown;
int[] speciesCount;
boolean lost;
particleRing particles;
organism algaeBP;
float countTime;
int score;
boolean reallySetup = true;
void setup() { //Creates stuff...
particles = new particleRing(0,newV(0,0),0,0,0);
countTime = 0;
powerupcooldown = 0;
powerup = new int[3];
powerUps = new PImage[5];
powdesc = new String[5];
powerup[0] = 0;
powerup[1] = 0;
powerup[2] = 0;
updatePowers();
updatePowers();
updatePowers();
score = 0;
//lostFade = 0;
powerUps[0] = loadImage("pup0.png");
powdesc[0] = "Well, it looks like you broke my code. Click.";
powerUps[1] = loadImage("pup1.png");
powdesc[1] = "Fattens the nearest fish.";
powerUps[2] = loadImage("pup2.png");
powdesc[2] = "Makes the nearest fish faster";
powerUps[3] = loadImage("pup3.png");
powdesc[3] = "Kill all algae in an area";
powerUps[4] = loadImage("pup4.png");
powdesc[4] = "Spawn lots of algae";
 speciesCount = new int[3];
if(reallySetup){
 size(640,480);
  minim = new Minim(this);
  music = minim.loadFile("Musac.mp3");
  music.play();
  music.loop();
  music.setGain(-20);
  reallySetup = false;
}
 background(0);
 specieColor = new ArrayList<color3>();
 specieColor.add(new color3(255,255,255));
  specieColor.add(new color3(0,255,255));
    specieColor.add(new color3(0,0,255));
    specieColor.add(new color3(255,0,255));

  
  specieSpawns = new ArrayList<vector2>();
  specieSpawns.add(newV(50,50));
  specieSpawns.add(newV(500,400));
dying = new ArrayList<organism>();
 children = new ArrayList<organism>(); //You know what's stupid? This. Ever heard of things that AREN'T pointers, Java?

 animals = new ArrayList<organism>(); //You know what's stupid? This. Ever heard of things that AREN'T pointers, Java?
 organism level = new organism(0,newV(50,50),newV(50,50),5,0.5,1,5,0,100,0,10,true,0);
 algaeBP = level;
 for(int i = 0;i < 40;++i){
   animals.add(new organism(level,randomV()));
 }
 level = new organism(1,newV(250,250),newV(50,50),10,1,6,0,0.021,5,10,10,true,0.5);
 for(int i = 0;i < 10;++i){
   animals.add(new organism(level,randomV()));
 }
 level = new organism(2,newV(250,250),newV(250,250),20,1.2,8,0,0.02,5,40,20,false,5);
 animals.add(level);
 for(int i = 0;i < animals.size();++i){
   animals.get(i).juvenilePause = 0;
 }
 herbivore = loadImage("herbivore.png");
 algae = loadImage("algae.png");
  predator = loadImage("predator.png");
lost = false;
 noStroke();
 speciesCount[0] = 100;
  speciesCount[1] = 100;
 speciesCount[2] = 100;

 for(int i = 0;i < animals.size();++i){
   speciesCount[animals.get(i).species]++;
 }
}
void keyPressed(){
  if(key == 'k'){
    save("screen.png");
  }
  if(key == 'p'){
    setup();
    lostFade = 0;
  }
}
void updatePowers(){
  for(int i = 0;i < 2;++i){
    powerup[i] = powerup[i+1];
  }
  powerup[2] = floor(random(1,5));
  powerupcooldown = 1;
  score++;
}
void exterminateAlgae(ArrayList<organism> b,vector2 p){
  for(int i = 0;i < b.size();++i){
    organism x = b.get(i);
  if(x.species != 0) continue;
  if(x.pos.subt(p).magnitudeSq() < sq(200)){
    x.girth = 0;
    b.remove(i);
    --i;
    speciesCount[0]--;
  }
  }
}
void recountPhoto(){
  totalPhoto = 0;
  for(int i = 0;i < animals.size();++i){
    totalPhoto += animals.get(i).photosynthesis;
  }
    for(int i = 0;i < children.size();++i){
    totalPhoto += children.get(i).photosynthesis;
  }
}
void mouseClicked() {
      vector2 mouseVec = newV(mouseX,mouseY);
  countAll();
  if(powerupcooldown > 0 || lost) return;
  if(powerup[0] == 1){
    organism closest = getClosest(animals,newV(mouseX,mouseY),-1,0);
    closest.girth += 10;
    closest.girthGoal += 10;
    updatePowers();
              particles = new particleRing(50,closest.pos,10,10,5);
    return;
  }
  if(powerup[0] == 2){
    organism closest = getClosest(animals,newV(mouseX,mouseY),-1,0);
    closest.speed += 1;
          particles = new particleRing(50,closest.pos,10,10,5);

    updatePowers();
    return;
  }
  if(powerup[0] == 3){
      particles = new particleRing(250,mouseVec,200,10,9);
    exterminateAlgae(animals,mouseVec);
    exterminateAlgae(children,mouseVec);
    exterminateAlgae(dying,mouseVec);
  recountPhoto();

    updatePowers();
    return;
  }
  if(powerup[0] == 4){
          particles = new particleRing(120,mouseVec,100,10,5);

    for(int i = 0;i < 100;++i){
      children.add(new organism(algaeBP,randomV(mouseVec,random(0,100))));
      totalPhoto += algaeBP.photosynthesis;
    }
      recountPhoto();

    updatePowers();
    return;
  }
  updatePowers();
  return;
}
void countAll(){
 speciesCount[0] = 0;
  speciesCount[1] = 0;
 speciesCount[2] = 0;

 for(int i = 0;i < animals.size();++i){
   speciesCount[animals.get(i).species]++;
 }
 
 for(int i = 0;i < children.size();++i){
   speciesCount[children.get(i).species]++;
 }
 for(int i = 0;i < 3;++i){
   if(speciesCount[i] == 0){lost=true;}
 }
}
void draw(){
  if(lostFade >= 1){
    background(0);
     fill(255);
     if(score > 0){
    text("You Lost\n\nBefore losing, you made " + score + " mutations.",100,40);
     }
       text("Press 'p' to play.\nInstructions: You are a planetary overseer, a person appointed by God\nto oversee the proper evolution of species. Specifically you must\nensure that no species ever goes extinct, for that would be a waste.\n\nTo do this, you are granted various mutations to apply to individuals and\nenvironmental changes. These can be used to shift the balance in favor of\na species or individual. Be warned that they may have long-standing consequences\nbecause the fittest organisms will reproduce and pass on their gifts.\nYour goal is to apply as many mutations as possible, advancing the species.\n\nYour species are: Algae, Fishy (Eats Algae), Pointy (Eats Fishy). \n(Although Fishy can eat Pointy if they're large. In general, big fish eat little fish)",100,200);
     
  return;  
}
  background(0,128,255);
  float newM = millis();
  float dT =(newM-lastM)*0.01;
  lastM = newM;
    for(int i = 0; i < children.size();++i){
    organism x = children.get(i);
    x.juvenilePause -= dT;
    if(x.juvenilePause <= 0){
      animals.add(x);
      children.remove(i);
    }
    if(x.species == 0){
          image(algae,x.pos.x-x.girth,x.pos.y-x.girth,x.girth*2,x.girth*2);
fill(0,128,255,((float)x.juvenilePause)/x.eggTime*255);
ellipse(x.pos.x,x.pos.y,x.girth*2+2,x.girth*2+2);

    }else{
        fill(255);
ellipse(x.pos.x,x.pos.y,x.girth*1.5,x.girth*2);
    }
  }
      for(int i = 0; i < dying.size();++i){
    organism x = dying.get(i);
    x.juvenilePause += dT;
    if(x.juvenilePause > x.eggTime){
      dying.remove(i);
    }
    if(x.species == 0){
          image(algae,x.pos.x-x.girth,x.pos.y-x.girth,x.girth*2,x.girth*2);
fill(0,128,255,((float)x.juvenilePause)/x.eggTime*255);
ellipse(x.pos.x,x.pos.y,x.girth*2+2,x.girth*2+2);
    }
  }
  for(int i = 0;i < animals.size();++i){
    organism x = animals.get(i);
    if(x.species == 0){
      image(algae,x.pos.x-x.girth,x.pos.y-x.girth,x.girth*2,x.girth*2);
    }
  }
  for(int i = 0; i < animals.size();++i){
    organism x = animals.get(i);
    if(x.species == 0){
    }else if(x.species == 1){
      pushMatrix();
      translate(x.pos.x,x.pos.y);
      rotate((x.fleeing()?0:PI)+atan2(x.pos.y-x.target.y,x.pos.x-x.target.x));
    image(herbivore,-x.girth,-x.girth,x.girth*2,x.girth*2);
    popMatrix();
    }else if(x.species == 2){
      pushMatrix();
      translate(x.pos.x,x.pos.y);
      rotate((x.fleeing()?0:PI)+atan2(x.pos.y-x.target.y,x.pos.x-x.target.x));
    image(predator,-x.girth,-x.girth,x.girth*2,x.girth*2);
    popMatrix();
    }
    x.update(dT);
  }
    for(int i = 0; i < children.size();++i){
    organism x = children.get(i);
    if(x.girth <= 2){
      totalPhoto -= x.photosynthesis;
      children.remove(i);
      --i;
      speciesCount[x.species]--;
      if(speciesCount[x.species] == 0){
      lost = true;
      }
    }
    }
  for(int i = 0; i < animals.size();++i){
    organism x = animals.get(i);
    if(x.girth <= 2){
      totalPhoto -= x.photosynthesis;
      animals.remove(i);
      --i;
            speciesCount[x.species]--;
      if(speciesCount[x.species] == 0){
      lost = true;
      }
    }
    if(x.foodLevel >= 2 && random(0,1) < 0.1){
      if(x.girth > x.girthGoal && x.birthPause <= (x.foodLevel + x.girth-x.girthGoal) && x.foodLevel > 1){
        if(x.species == 2) println(x.girth);
           if(animals.size() + children.size() > 1000) continue;
      x.girth *= 0.75;
      totalPhoto += x.photosynthesis;
      println(totalPhoto);
      children.add(new organism(x,randomV(x.pos,x.girth*2+x.birthRange)));
      if(totalPhoto > 500 && x.photosynthesis > 0){
        totalPhoto -= x.photosynthesis;
        animals.remove(i);
        dying.add(x);
      }
      x.foodLevel -= 10;
      x.birthPause = 10;
      }else if(x.girth < x.girthGoal*1.5){
        x.girth++;
        x.foodLevel-=2;
      }
    }
    if(x.foodLevel < -10){
      x.girth--;
      x.foodLevel += 1;
    }
  }

  fill(0);
  rect(0,height-controlsSize,width,height);
  for(int i = 0;i < 3;++i){
    image(powerUps[powerup[i]],14+57*i,height-controlsSize+7,50,50);
  }
  fill(255);
    text(powdesc[powerup[0]],185,height-controlsSize+7,100,100);
    text("Score: " + score,500,height-controlsSize+7,100,100);
if(powerupcooldown > 0){
  fill(255,0,0,128);
  rect(14,height-controlsSize+7+powerupcooldown*50,50,(1-powerupcooldown)*50);
  powerupcooldown -= dT/5;
}else{
  fill(255,255,0,128);
  rect(14,height-controlsSize+7,50,50);
}
countTime -= dT;
if(countTime <= 0){
  countTime = 2;
  recountPhoto();
  countAll();
}
if(lost){
  lostFade += dT/10;
  fill(0,0,0,lostFade*255);
  rect(0,0,width,height);
}
particles.updateDraw(dT);
}
