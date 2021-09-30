import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;

UpgradeStage now;
GameStage gam;
PImage heartImage;
PImage plyrImage;
PImage chestImage;
PImage slimeImage;
PImage slimerImage;
PImage slimeCharImage;
PImage bomberCharImage;
PImage zombieCharImage;
PImage tankImage;
PImage swordImage;

Minim minim;
AudioSnippet gun;
AudioSnippet healthGain;
AudioSnippet chestDrop;
AudioSnippet boom;
AudioSnippet buySound;
AudioSnippet slimeSound;
AudioSnippet hurtSound;
AudioSnippet slashSound;

boolean started;
AudioPlayer music;
int scoreEnd;
void setup(){
  started = false;
    minim = new Minim(this);
gun = minim.loadSnippet("gunshot.wav");
healthGain = minim.loadSnippet("healthpickup.wav");
healthGain.setGain(-10);
chestDrop = minim.loadSnippet("chest.wav");
chestDrop.setGain(-10);
boom = minim.loadSnippet("bomb.wav");
boom.setGain(-10);
buySound = minim.loadSnippet("purchase.wav");
buySound.setGain(-5);
slimeSound = minim.loadSnippet("slime.wav");
slimeSound.setGain(-5);
hurtSound = minim.loadSnippet("hurt.wav");
hurtSound.setGain(-10);
slashSound = minim.loadSnippet("slash.wav");
slashSound.setGain(-5);

  music = minim.loadFile("stabystab.mp3");
  music.play();
  music.loop();
  music.setGain(-5);
  size(640,480);
  
  plyrImage = loadImage("char1.png");
  heartImage = loadImage("health.png");
  chestImage = loadImage("chest.png");
  slimeImage = loadImage("slime.png");
    slimerImage = loadImage("slimer.png");
tankImage = loadImage("tank.png");
    slimeCharImage = loadImage("slimechar.png");
bomberCharImage = loadImage("bombman.png");
zombieCharImage = loadImage("zombie.png");
swordImage = loadImage("sword.png");

  background(255);
  now = new UpgradeStage();
  now.purchases.add(new StarterGun());
  now.plyr = new Player(320,240,50,10);
  now.lastToSpawn = new ArrayList<Enemy>();
  for(int i = 0;i < 6;i++){
        now.lastToSpawn.add(new Zombie(random(-10,width+10),-10,10,9,2));
  }
  /*frameRate(40);
   for(int i = 0;i < 1200;i++){
        Vector2 v = onEdge(10);
        now.enemiesToSpawn.add(new Crowder(v.x,v.y,10,100,2+random(1)));*/

}
Vector2 onEdge(float rad){
    Vector2 out = new Vector2(-rad,-rad);
    if(random(width+height+rad+rad)>width+rad){
      if(random(1) > 0.5){
        out.x = width+rad;
      }
      out.y = random(-rad,height+rad);
    }else{
      if(random(1) > 0.5){
        out.y = height+rad;
      }
      out.x = random(-rad,width+rad);
    }
    return out;
  }
  boolean debounceDead = false;
void draw(){
if(started){
  if(now==null){
    fill(255);
    background(0);
    textAlign(CENTER,CENTER);
    text("DEAD\nScore: "  + scoreEnd,width/2,height/2);
        textAlign(RIGHT,BOTTOM);
    textSize(30);
    if(!mousePressed) debounceDead = true;
    if(mouseY > height - 40 && mouseX > width - 165){
      if(mousePressed && debounceDead){
debounceDead = true;
  now = new UpgradeStage();
  now.purchases.add(new StarterGun());
  now.plyr = new Player(320,240,50,10);
  now.lastToSpawn = new ArrayList<Enemy>();
  for(int i = 0;i < 6;i++){
        now.lastToSpawn.add(new Zombie(random(-10,width+10),-10,10,9,2));
  }
      }else{
        fill(255,255,0);
      }
    }
    text("Try Again",width-5,height-5);
  }else
  if(!now.goTime){
  //now.update();
  now.render();
  }else{
    if(gam == null) gam = now.go();
    gam.update();
    gam.render();
    if(gam.done){
      now = gam.go();
      scoreEnd = gam.score;
    if(gam.dead) now = null;
      gam = null;
    }
  }
}else{
      fill(255);
    background(0);
        textSize(20);

    textAlign(CENTER,CENTER);
    text("Power Grab!\nAs a villain, you have some enemies. You should probably, like deal with them somehow, lie villains do. But to do that, you need to use artifacts, and that attracks more enemies. So maybe don't take as many artifacts as possible.\n\nPress any key to continue.",10,0,width-20,height);
    if(keyPressed) started = true;
}
}
void keyPressed(){
  if(gam != null){
    if(key==CODED){
      if(keyCode==UP){
        gam.plyr.kup=true;
      }else if(keyCode==DOWN){
        gam.plyr.kdown=true;
      }else if(keyCode==LEFT){
        gam.plyr.kleft=true;
      }else if(keyCode==RIGHT){
        gam.plyr.kright=true;
      }
    }else{
          if(key=='w' || key=='W'){
        gam.plyr.kup=true;
      }else if(key=='a' || key=='A'){
        gam.plyr.kleft=true;
      }else if(key=='s' || key == 'S'){
        gam.plyr.kdown=true;
      }else if(key=='d' || key == 'D'){
        gam.plyr.kright =true;
      }
    }
  }
}
void keyReleased(){
  if(key == 'q') saveFrame("screenshot-##.png");
  if(gam != null){
    if(key==CODED){
      if(keyCode==UP){
        gam.plyr.kup=false;
      }else if(keyCode==DOWN){
        gam.plyr.kdown=false;
      }else if(keyCode==LEFT){
        gam.plyr.kleft=false;
      }else if(keyCode==RIGHT){
        gam.plyr.kright=false;
      }
    }else{
          if(key=='w' || key == 'W'){
        gam.plyr.kup=false;
      }else if(key=='a' || key =='A'){
        gam.plyr.kleft=false;
      }else if(key=='s' || key == 'S'){
        gam.plyr.kdown=false;
      }else if(key=='d' || key == 'D'){
        gam.plyr.kright=false;
      }
    }
  }
}
