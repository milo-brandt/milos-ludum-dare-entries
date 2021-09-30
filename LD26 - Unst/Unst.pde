import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;

grid main;
grid mainR;
grid[] levels;
int levelN = 0;
float anVal=0;
int anT=0;
int lastTeam=-1;
PImage[] pos;
PImage[] neg;
String[] levelNames;
Minim minim;
AudioPlayer music;

void setup(){
    minim = new Minim(this);
  music = minim.loadFile("themePush.mp3");
  music.play();
  music.loop();
  music.setGain(-10);
  pos = new PImage[4];
  pos[0] = loadImage("downg.png");
  pos[1] = loadImage("upg.png");
  pos[2] = loadImage("rightg.png");
  pos[3] = loadImage("leftg.png");
  neg = new PImage[4];
  neg[0] = loadImage("downb.png");
  neg[1] = loadImage("upb.png");
  neg[2] = loadImage("rightb.png");
  neg[3] = loadImage("leftb.png");

  int l = 0;
  levels = new grid[10];
  levelNames = new String[10];
  levelNames[l] = "Click";
  levels[l] = new grid(3,3,1);
  levels[l].type[3]=1;
  levels[l].team[3]=0;
  
  levels[l].teamDir[0] = 2;
  levels[l].x1=2;
  levels[l].x2=3;
  levels[l].y1=1;
  levels[l].y2=2;
  l++;
  
      
  levelNames[l] = "Follow Me";
  levels[l] = new grid(5,5,2);
  levels[l].type[11]=1;
  levels[l].team[11]=0;

  levels[l].type[10]=1;
  levels[l].team[10]=1;

  levels[l].teamDir[0] = 2;
  levels[l].teamDir[1] = 2;
  levels[l].x1=3;
  levels[l].x2=5;
  levels[l].y1=2;
  levels[l].y2=3;

  l++;
  
  levelNames[l] = "'r' to restart";
  levels[l] = new grid(5,5,2);
  levels[l].type[5]=1;
  levels[l].team[5]=0;
  levels[l].type[6]=1;
  levels[l].team[6]=0;


  levels[l].type[2]=1;
  levels[l].team[2]=1;
  levels[l].type[3]=1;
  levels[l].team[3]=1;


  levels[l].teamDir[0] = 2;
  levels[l].teamDir[1] = 0;
  levels[l].x1=2;
  levels[l].x2=4;
  levels[l].y1=1;
  levels[l].y2=3;
  
  l++;
  levelNames[l] = "Liquidation";
  levels[l] = new grid(3,3,3);
  levels[l].type[1]=1;
  levels[l].team[1]=0;
  levels[l].type[2]=1;
  levels[l].team[2]=0;

  levels[l].type[0]=-1;
  levels[l].team[0]=1;

  levels[l].type[5]=1;
  levels[l].team[5]=2;

  levels[l].teamDir[0] = 0;
  levels[l].teamDir[1] = 2;
  levels[l].teamDir[2] = 3;
  
  levels[l].x1=1;
  levels[l].x2=3;
  levels[l].y1=1;
  levels[l].y2=2;
  
  l++;
  levelNames[l] = "Banana Split";

  levels[l] = new grid(7,7,4);
  levels[l].type[15]=1;
  levels[l].team[15]=0;
  levels[l].type[22]=1;
  levels[l].team[22]=0;
  levels[l].type[29]=1;
  levels[l].team[29]=0;
  
  levels[l].type[24]=-1;
  levels[l].team[24]=1;
  levels[l].type[25]=1;
  levels[l].team[25]=1;
  levels[l].type[26]=1;
  levels[l].team[26]=1;

  levels[l].type[14]=1;
  levels[l].team[14]=2;

  levels[l].type[34]=1;
  levels[l].team[34]=3;


  levels[l].teamDir[0] = 2;
  levels[l].teamDir[1] = 3;
  levels[l].teamDir[2] = 2;
  levels[l].teamDir[3] = 3;

  levels[l].x1=2;
  levels[l].x2=4;
  levels[l].y1=2;
  levels[l].y2=5;
  
    l++;
  levelNames[l] = "The Chosen One";
 
  levels[l] = new grid(9,9,3);
  levels[l].setCell(0,3,1,0);
  levels[l].setCell(1,3,1,0);
  levels[l].setCell(2,3,1,0);
  levels[l].setCell(0,4,1,0);
  levels[l].setCell(1,4,-1,0);
  levels[l].setCell(2,4,1,0);
  levels[l].setCell(0,5,1,0);
  levels[l].setCell(1,5,1,0);
  levels[l].setCell(2,5,1,0);
  levels[l].teamDir[0] = 2;
  
  levels[l].setCell(3,0,-1,1);
  levels[l].setCell(5,0,1,1);
  levels[l].setCell(4,0,-1,1);
  levels[l].setCell(4,1,1,1);
  levels[l].setCell(4,2,-1,1);
  levels[l].teamDir[1] =0 ;

  levels[l].setCell(6,3,-1,2);
  levels[l].setCell(7,3,-1,2);
  levels[l].setCell(7,4,-1,2);
  levels[l].setCell(7,5,-1,2);
  levels[l].setCell(8,5,-1,2);
  levels[l].teamDir[2] =3 ;

  levels[l].x1=4;
  levels[l].x2=5;
  levels[l].y1=4;
  levels[l].y2=5;
  l++;
  
    levelNames[l] = "Tetris";
 
  levels[l] = new grid(15,15,5);
  levels[l].setCell(7,0,1,0);
  levels[l].setCell(8,0,1,0);
  levels[l].setCell(8,1,1,0);
  levels[l].setCell(8,2,1,0);
  levels[l].setCell(8,2,1,0);
  levels[l].setCell(8,3,-1,0);
  levels[l].setCell(8,4,-1,0);
  levels[l].setCell(8,5,-1,0);

  levels[l].teamDir[0] = 0;
  
   levels[l].setCell(9,6,1,1);
  levels[l].setCell(10,6,1,1);
    levels[l].setCell(10,7,1,1);

  levels[l].setCell(11,6,1,1);
  levels[l].setCell(11,7,1,1);
  levels[l].setCell(12,7,1,1);
  levels[l].teamDir[1] = 3;
  
  levels[l].setCell(5,10,1,2);
  levels[l].setCell(5,11,-1,2);
  levels[l].setCell(5,12,1,2);
  levels[l].setCell(5,13,1,2);
  levels[l].setCell(5,14,1,2);
  levels[l].setCell(6,14,1,2);
  levels[l].teamDir[2] = 1;
  
  levels[l].setCell(1,8,1,3);
  levels[l].setCell(1,9,-1,3);
  levels[l].setCell(2,8,1,3);
  levels[l].setCell(2,9,1,3);
  levels[l].setCell(3,8,1,3);
  levels[l].setCell(3,9,1,3);
  levels[l].teamDir[3] = 2;

  levels[l].setCell(8,11,1,4);
  levels[l].teamDir[4] = 0;
  
  levels[l].x1=5;
  levels[l].x2=9;
  levels[l].y1=6;
  levels[l].y2=10;
  l++;
  levelNames[l] = "Total Annihilation";
 
  levels[l] = new grid(8,8,5);
  levels[l].setCell(2,1,1,0);
  levels[l].setCell(2,2,1,0);
  levels[l].setCell(2,3,-1,0);
  levels[l].setCell(3,3,1,0);
  levels[l].teamDir[0] = 2;

  levels[l].setCell(5,0,-1,1);
  levels[l].setCell(5,1,-1,1);
  levels[l].setCell(5,2,-1,1);
  levels[l].setCell(5,3,1,1);
  levels[l].setCell(4,3,1,1);
  levels[l].teamDir[1] = 3;

  levels[l].setCell(2,4,1,2);
  levels[l].setCell(2,5,-1,2);
  levels[l].setCell(3,5,1,2);
  levels[l].setCell(4,5,1,2);
  levels[l].setCell(5,5,-1,2);
  levels[l].setCell(5,4,1,2);
  levels[l].setCell(5,6,1,2);
  levels[l].teamDir[2] = 1;
  
  levels[l].setCell(6,2,-1,3);
  levels[l].setCell(6,3,1,3);
  levels[l].setCell(7,2,1,3);
  levels[l].setCell(7,3,-1,3);
  levels[l].teamDir[3] = 3;
  
  levels[l].setCell(3,6,-1,4);
  levels[l].setCell(3,7,-1,4);
  levels[l].setCell(4,6,-1,4);
  levels[l].setCell(4,7,-1,4);
  levels[l].teamDir[4] = 1;
    
  levels[l].x1=-5;
  levels[l].x2=-5;
  levels[l].y1=-5;
  levels[l].y2=-5;
  l++;

  levelNames[l] = "Gettin' Real";
  levels[l] = new grid(9,9,6);
  
  levels[l].setCell(1,4,1,0);
  levels[l].teamDir[0] = 2;

  levels[l].setCell(2,7,1,1);
  levels[l].setCell(3,7,1,1);
  levels[l].teamDir[1] = 1;

  levels[l].setCell(4,0,-1,2);
  levels[l].setCell(4,1,-1,2);
  levels[l].setCell(5,1,-1,2);
  levels[l].teamDir[2] = 0;

  levels[l].setCell(5,3,1,3);
  levels[l].teamDir[3] = 3;

  levels[l].setCell(4,6,1,4);
  levels[l].teamDir[4] = 1;
  
  levels[l].setCell(6,3,1,5);
  levels[l].setCell(7,3,1,5);
  levels[l].setCell(8,3,1,5);
  levels[l].setCell(7,4,1,5);
  levels[l].setCell(6,5,1,5);
  levels[l].setCell(7,5,1,5);
  levels[l].setCell(8,5,1,5);
  levels[l].teamDir[5] = 3;

  levels[l].x1 = 2;
  levels[l].y1 = 3;
  levels[l].x2 = 5;
  levels[l].y2 = 6;
  l++;
  levelNames[l] = "Finale";
  levels[l] = new grid(15,15,7);
  
  levels[l].setCell(2,0,1,0);
  levels[l].setCell(2,1,1,0);
  levels[l].setCell(3,1,1,0);
  levels[l].setCell(4,1,1,0);
  levels[l].setCell(4,2,-1,0);
  levels[l].teamDir[0] = 0;
  
  levels[l].setCell(3,2,-1,1);
  levels[l].teamDir[1] = 0;

  levels[l].setCell(1,2,1,2);
  levels[l].setCell(2,2,1,2);
  levels[l].setCell(1,3,1,2);
  levels[l].setCell(1,4,-1,2);
  levels[l].setCell(2,4,-1,2);
  levels[l].setCell(2,5,1,2);
  levels[l].setCell(2,6,1,2);
  levels[l].setCell(3,6,1,2);
  levels[l].setCell(3,4,1,2);
  levels[l].setCell(4,4,1,2);
  levels[l].setCell(5,4,-1,2);
  levels[l].setCell(5,3,1,2);
  levels[l].setCell(5,2,1,2);
  levels[l].setCell(6,2,1,2);
  levels[l].setCell(7,2,1,2);
  levels[l].teamDir[2] = 3;

  levels[l].setCell(6,4,1,3);
  levels[l].setCell(7,4,1,3);
  levels[l].setCell(8,4,1,3);
  levels[l].setCell(9,4,1,3);
  levels[l].setCell(10,4,1,3);
  levels[l].setCell(11,4,1,3);
  levels[l].teamDir[3] = 3;
  
  levels[l].setCell(8,5,1,4);
  levels[l].setCell(9,5,1,4);
  levels[l].teamDir[4] = 3;

  levels[l].setCell(4,6,-1,5);
  levels[l].setCell(4,7,1,5);
  levels[l].setCell(3,9,-1,5);
  levels[l].setCell(4,8,1,5);
  levels[l].setCell(3,8,-1,5);
  levels[l].setCell(2,10,1,5);
  levels[l].setCell(2,9,1,5);
  levels[l].teamDir[5] = 1;
  
  levels[l].setCell(3,10,1,6);
  levels[l].setCell(3,11,1,6);
  levels[l].setCell(3,12,1,6);
  levels[l].setCell(3,13,1,6);
  levels[l].setCell(4,13,1,6);
  levels[l].teamDir[6] = 1;

  levels[l].x1 = 0;
  levels[l].y1 = 2;
  levels[l].x2 = 5;
  levels[l].y2 = 7;
  
  size(500,500);
  mainR = levels[levelN];
  main = new grid(mainR);
}
void draw(){
  background(0);
  noStroke();
  if(0 < mouseX && mouseX < 500 && 0 < mouseY && mouseY < 500){
  main.highTeam = main.team[floor(mouseX*main.w/500)+main.w*floor(mouseY*main.h/500)];
  }
  main.render();
  if(main.hasWon() && anT != 1){
    anT = 1;
    anVal = 0;/*
    levelN++;
    mainR = levels[levelN];
    main = new grid(mainR);*/
  }
  fill(255);
  textSize(40);
  textAlign(CENTER,BOTTOM);
  text(levelNames[levelN],250,490);
  if(anT != 0){
    anVal += 1.0/15;
    if(anT == 1){
      fill(0,0,0,anVal*64);
      rect(0,0,500,500);
      if(anVal >= 4){
        levelN++;
        if(levelN == levels.length){
          levelN--;
          fill(255);
          textAlign(CENTER,CENTER);
            textSize(50);

          text("The End!",250,250);
        }else{
        mainR = levels[levelN];
        main = new grid(mainR);
        anT = 0;
        anT = 2;
        anVal = 0;
        }
      }
                  strokeWeight(4);
    stroke(0,255,0,min(anVal*255,255*4-anVal*255));
    line(main.x1*500/main.w,main.y1*500/main.h,main.x2*500/main.w,main.y1*500/main.h);
    line(main.x1*500/main.w,main.y1*500/main.h,main.x1*500/main.w,main.y2*500/main.h);
    line(main.x2*500/main.w,main.y1*500/main.h,main.x2*500/main.w,main.y2*500/main.h);
    line(main.x1*500/main.w,main.y2*500/main.h,main.x2*500/main.w,main.y2*500/main.h);

    }
    if(anT == 2){
      fill(0,0,0,255-anVal*128);
      rect(0,0,500,500);
      if(anVal >= 2){
        anT = 0;
        anVal = 0;
      }
    }
  }
}
void mousePressed(){
  if(anT == 1) return;
  anVal = 0;
  if(main.highTeam == main.moveTeam) return;
  if(main.highTeam == -1) return;
  if(main.canAdvanceTeam(main.highTeam)){
    if(main.moveTeam != -1){
      main.advanceTeam(main.moveTeam);
      main.moveTeam = main.highTeam;
      main.anV = 0;
    }else{
      main.moveTeam = main.highTeam;
      main.anV = 0;
    }
  }
}
void keyPressed(){
  if(anT == 1) return;
  if(key=='r') main=new grid(mainR);
  if(key=='k') saveFrame("screen");
}
