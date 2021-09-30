  import ddf.minim.*;

Level main;
boolean go;
Level make(int n){
  if(n==0){
    Level m = new Level(new PlayerData(10,new vector2(100,490),new vector2(150,100),.01,0));
    m.addRect(50,500,700,550);
    //m.addRect(700,0,800,400);
    //m.addRect(0,0,50,400);
    m.addFlag(new vector2(600,490));
    m.stage.beginDraw();
    m.stage.filter(BLUR,3);
    m.stage.textAlign(CENTER,TOP);
    m.stage.textSize(50);
    m.stage.fill(255);
    m.stage.text("Press 'r' to Restart",width/2,50);
    m.stage.endDraw();
    return m;
  }else if(n==1){
    Level m = new Level(new PlayerData(10,new vector2(100,490),new vector2(150,100),.01,0));
    m.addRect(50,500,300,550);
    m.addRect(450,500,700,550);
    //m.addRect(700,0,800,400);
    //m.addRect(0,0,50,400);
    m.addFlag(new vector2(600,490));
    m.stage.beginDraw();
    m.stage.filter(BLUR,3);
    m.stage.textAlign(CENTER,TOP);
    m.stage.textSize(50);
    m.stage.fill(255);
    m.stage.text("Press Space Bar to Jump",width/2,50);
    m.stage.endDraw();
    return m;
  }else if(n==2){
    Level m = new Level(new PlayerData(10,new vector2(100,490),new vector2(150,100),.01,0));
    m.addRect(50,550,700,500);
    m.addRect(700,0,800,550);
    m.addRect(50,200,650,450);
    m.addFlag(new vector2(100,190));
        m.stage.filter(BLUR,3);

    return m;
  }else if(n==3){
    Level m = new Level(new PlayerData(10,new vector2(100,190),new vector2(150,100),.01,0));
    m.addRect(50,550,700,500);
    m.addRect(700,200,800,550);
    m.addRect(50,200,640,400);
    m.addFlag(new vector2(100,490));
    m.stage.beginDraw();
    m.stage.filter(BLUR,3);
    m.stage.textAlign(CENTER,TOP);
    m.stage.textSize(50);
    m.stage.fill(255);
    m.stage.text("Hold Down Arrow to Descend",width/2,50);
    m.stage.endDraw();
    return m;
  }else if(n==4){
    Level m = new Level(new PlayerData(10,new vector2(100,490),new vector2(150,100),.01,0));
    m.addRect(50,550,700,500);
    m.addRect(700,100,800,550);
    m.addRect(50,200,300,450);
    m.addRect(350,200,650,450);
    m.stage.beginDraw();
    m.stage.filter(BLUR,3);
    m.stage.textAlign(CENTER,TOP);
    m.stage.textSize(50);
    m.stage.fill(255);
    m.stage.text("You can't cross rainbows",width/2,50);
    m.stage.endDraw();

    m.addFlag(new vector2(450,190));
    m.addFlag(new vector2(75,490));
    return m;
  }else if(n==5){
    Level m = new Level(new PlayerData(10,new vector2(100,490),new vector2(150,100),.01,0));
    m.addRect(50,550,700,500);
    m.addRect(700,100,800,550);
    m.addRect(225,100,300,450);
     m.addRect(300,100,700,150);
    m.addRect(350,200,650,450); 
    m.addRect(50,150,150,350);
    m.stage.beginDraw();
    m.stage.filter(BLUR,3);
    m.stage.textAlign(CENTER,TOP);
    m.stage.textSize(40);
    m.stage.fill(255);
    m.stage.text("Hold Down Longer to Keep Bouncing",width/2,20);
    m.stage.endDraw();

    m.addFlag(new vector2(450,190));
    m.addFlag(new vector2(75,140));
    return m;
  }else if(n==6){
    Level m = new Level(new PlayerData(10,new vector2(400,490),new vector2(150,100),.01,0));
    m.addRect(50,500,700,550);
    m.addRect(700,0,800,550);
    m.addRect(0,0,50,550);
    m.addFlag(new vector2(400,350));
        m.stage.filter(BLUR,3);

    return m;
  }else if(n==7){
    Level m = new Level(new PlayerData(10,new vector2(450,490),new vector2(150,100),.01,0));
    m.addRect(50,500,300,550); //Left down
    m.addRect(450,500,750,550); //Right down
    m.addRect(50,300,250,310); //Left mid
    m.addRect(500,300,700,400); //Right mid
    m.addRect(100,100,250,200); //Left top
    m.addRect(500,100,750,110); //Right top
 
    //m.killers.add(new Line(new vector2(251,300),new vector2(251,400)));
    //m.killers.add(new Line(new vector2(499,300),new vector2(499,400)));
    //m.killers.add(new Line(new vector2(251,100),new vector2(251,200)));
    //m.killers.add(new Line(new vector2(499,100),new vector2(499,200)));


    m.addRect(750,0,800,550); //Wall right
    m.addRect(0,0,50,550); //Wall left
    
        m.addFlag(new vector2(725,350));
    m.addFlag(new vector2(75,150));
    m.addFlag(new vector2(725,90));
        m.stage.filter(BLUR,3);

    return m;
  }else if(n==8){
    Level m = new Level(new PlayerData(10,new vector2(400,490),new vector2(150,100),.01,0));
    m.addRect(400,550,750,500); //Platform
    m.addRect(750,0,800,550); //Back wall
    m.addRect(600,100,700,390); //High platform
    m.addRect(690,390,700,425); //High platform

    m.addRect(510,0,550,350); //Chute 
    m.addRect(350,0,400,350); //Chute
    m.addRect(400,0,510,50); //Chute
    
    
    m.addRect(0,500,150,550); //Landing
    //m.addObject(new Line(new vector2(350,350),new vector2(50,425)));
    m.addObject(new Parabola(1/900.0,-7/9.0,4375/9.0,50,350));
    m.addRect(0,0,50,500); //Landing
    m.stage.beginDraw();
    m.stage.fill(255);
    m.stage.noStroke();
    m.stage.beginShape(TRIANGLE_FAN);
    m.stage.vertex(350,0);
    m.stage.vertex(50,0);
    for(int i = 50;i<=350;++i){
      m.stage.vertex(i,i*i/900.f-i*7/9.f+4375/9.f);
    }
    m.stage.endShape();
        m.stage.filter(BLUR,3);

    m.stage.endDraw();

    m.addFlag(new vector2(75,475));
    return m; 
  }
  return null;
}
int level = 0;

  Minim minim;
AudioPlayer backgroundmusic;
AudioSample boing;
AudioPlayer ding;
PImage flag;

void setup(){
  size(800,600,P2D);
  frameRate(100);
  main = make(level);
  textAlign(CENTER,TOP);
  textSize(50);
minim = new Minim(this);
backgroundmusic = minim.loadFile("Gamemusic.mp3");
boing = minim.loadSample("boing.mp3",512);
ding = minim.loadFile("star.mp3");
backgroundmusic.loop();
flag = loadImage("star.png");
go = false;
}
void draw(){
  background(255);
   main.Draw();
   if(!go){
     fill(255);
     text("Press any key to start.",width/2,150);
     if(keyPressed){
       keyPressed = false;
       go = true;
     }else return;
   }
  if(main.RunStep()){
    fill(0);
    
    if(level == 8){ text("You win!",width/2-200,50); return;}
    main = make(++level);
      color(255,255,0);
  }
}
void keyPressed() {
  if(key == 'r'){ main = make(level);

  keyPressed=false;}
   if(key == 'w'){ saveFrame();

  keyPressed=false;}
} 