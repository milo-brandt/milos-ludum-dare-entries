import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import ddf.minim.*; 
import ddf.minim.signals.*; 
import ddf.minim.analysis.*; 
import ddf.minim.effects.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Unst extends PApplet {






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

public void setup(){
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
public void draw(){
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
    anVal += 1.0f/15;
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
public void mousePressed(){
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
public void keyPressed(){
  if(anT == 1) return;
  if(key=='r') main=new grid(mainR);
}
class grid{
  public int[] type;
  public int[] team;
  public int[] teamDir;
  int w,h;
  int highTeam;
  int x1,y1,x2,y2;
  int moveTeam;
  float anV;
  grid(int w,int h,int t){
    this.w = w;
    this.h = h;
    moveTeam = -1;
    anV = 0;
    highTeam = -1;
    type = new int[w*h];
    team = new int[w*h];
    for(int i = 0;i < w*h;++i){
      type[i]=0;
      team[i]=-1;
    }
    teamDir = new int[t];
    for(int i = 0;i < t;++i){
      teamDir[i]=0;
    }
  }
  grid(grid b){
    this.w = b.w;
    this.h = b.h;
    this.highTeam = b.highTeam;
    this.x1 = b.x1;
    this.x2 = b.x2;
    this.y1 = b.y1;
    this.y2 = b.y2;
    this.type = new int[b.type.length];
    this.team = new int[b.team.length];
    this.teamDir = new int[b.teamDir.length];
    moveTeam = -1;
    anV = 0;
    for(int i = 0;i < type.length;++i){
      type[i] = b.type[i];
    }
    for(int i = 0;i < team.length;++i){
      team[i] = b.team[i];
    }
    for(int i = 0;i < teamDir.length;++i){
      teamDir[i] = b.teamDir[i];
    }
  }
  public int getP(int x,int y){
    return x+y*w;
  }
  public void reteam(int t){
    int[] nteams = new int[w*h];
    int n = 0;
    for(int i = 0;i < w*h;++i){
      nteams[i]=-1;
    }
    for(int i = 0;i < w*h;++i){
      if(team[i]==t&&nteams[i]==-1){
        ArrayList op = new ArrayList();
        op.add(i);
        while(!op.isEmpty()){
          int f = (Integer)op.get(0);
          op.remove(0);
          if(nteams[f]!=-1) continue;
          if(team[f]!= t) continue;
          nteams[f] = n;
          if(f%w!=0) op.add(f-1);
          if((f+1)%w!=0) op.add(f+1);
          if(f>=w) op.add(f-w);
          if(f<w*(h-1)) op.add(f+w);
        }
        ++n;
      }
    }
    if(n<=1) return;
    println(n);
    int[] nteamDir = new int[teamDir.length + n - 1];
    for(int i = 0;i < teamDir.length;++i) nteamDir[i] = teamDir[i];
    for(int i = teamDir.length;i < teamDir.length + n - 1;++i) nteamDir[i] = teamDir[t];
    for(int i = 0;i < w*h;++i){
      if(nteams[i]>0){
        team[i] = teamDir.length+nteams[i]-1;
      }
    }
    teamDir = nteamDir;
  }
  public boolean canA(int i,int d){
    if(d==0){ //Down
      if(i>=w*(h-1)) return false;
      if(team[i+w]==team[i]) return true;
      if(type[i+w]==type[i]) return false;
      return true;
    }else if(d==1){ //Up
      if(i<w) return false;
      if(team[i-w]==team[i]) return true;
      if(type[i-w]==type[i]) return false;
      return true;
    }else if(d==2){ //Right
      if((i+1)%w==0) return false;
      if(team[i+1]==team[i]) return true;
      if(type[i+1]==type[i]) return false;
      return true;
    }else if(d==3){ //Left
      if(i%w==0) return false;
      if(team[i-1]==team[i]) return true;
      if(type[i-1]==type[i]) return false;
      return true;
    }
    return true;
  }
  public boolean canAdvanceTeam(int t){
    int d = teamDir[t];
    for(int i = 0;i < w*h;++i){
      if(team[i]==t && !canA(i,d)) return false;
    }
    return true;
  }
  public void setCell(int x,int y,int ty,int te){
    team[getP(x,y)]=te;
    type[getP(x,y)]=ty;
  }
  public void advanceTeam(int t){
    int d = teamDir[t];
    if(d==0){ //Down
      for(int i = w*h-1;i >= w;--i){
        if(team[i]==t){
          type[i] = 0;
          team[i] = -1;
        }
        if(team[i-w]==t){
          type[i] += type[i-w];
          team[i] = team[i-w];
          if(type[i]==0) team[i]=-1;
        }
      }
      for(int i = w-1;i >= 0;--i){
        if(team[i]==t){
          type[i] = 0;
          team[i] = -1;
        }
      }
    }else if(d==1){ //Up
      for(int i = 0;i < w*(h-1);++i){
        if(team[i]==t){
          type[i] = 0;
          team[i] = -1;
        }
        if(team[i+w]==t){
          type[i] += type[i+w];
          team[i] = team[i+w];
          if(type[i]==0) team[i]=-1;
        }
      }
      for(int i =  w*(h-1);i < w*h;++i){
        if(team[i]==t){
          type[i] = 0;
          team[i] = -1;
        }
      }
    }else if(d==2){ //Right
      for(int i = w*h-1;i >= 0;--i){
        if(team[i]==t){
          type[i] = 0;
          team[i] = -1;
        }
        if(i%w != 0){
          if(team[i-1]==t){
            type[i] += type[i-1];
            team[i] = team[i-1];
            if(type[i]==0) team[i]=-1;
          }
        }
      }
    }else if(d==3){ //Left
      for(int i = 0;i < w*h;++i){
        if(team[i]==t){
          type[i] = 0;
          team[i] = -1;
        }
        if((i+1)%w != 0){
          if(team[i+1]==t){
            type[i] += type[i+1];
            team[i] = team[i+1];
            if(type[i]==0) team[i]=-1;
          }
        }
      }
    }
    for(int i = teamDir.length-1;i>=0;--i) reteam(i);
  }
  public boolean hasWon(){
    for(int x = x1;x < x2;++x){
      for(int y = y1;y < y2;++y){
        if(type[y*w+x]!=1) return false;
      }
    }
    for(int i = 0;i < w*h;++i){
      if(type[i]==-1)return false;
    }
    return true;
  }
  public int fixT(int t){
    if(t == moveTeam) return -1;
    return t;
  }
  public void render(){
    if(moveTeam != -1){
      anV += 1.0f/30;
      if(anV >= 1){
        advanceTeam(moveTeam);
        moveTeam = -1;
        anV = 0;
      }
    }
    noStroke();
    background(0);
    boolean highMove = (highTeam!=-1)?canAdvanceTeam(highTeam):false;
    for(int x = 0;x < w;++x){
      for(int y = 0;y < h;++y){
        int t = main.type[getP(x,y)];
        /*if(highTeam != -1 && team[getP(x,y)]==highTeam){
          if(highMove){
            if(t==0) fill(255);
            if(t==1) fill(64,64,0);
            if(t==-1) fill(255,64,0);
          }else{
            if(t==0) fill(255);
            if(t==1) fill(64);
            if(t==-1) fill(196,0,0);
          }
        }else{
          if(t==0) fill(255);
          if(t==1) fill(0);
          if(t==-1) fill(255,0,0);
        }*/
        if(moveTeam != -1 && moveTeam == team[getP(x,y)]){
          int d = teamDir[team[getP(x,y)]];
          pushMatrix();
          if(d==0){
            translate(0,anV*500/h);
          }else if(d==1){
            translate(0,-anV*500/h);
          }else if(d==2){
            translate(anV*500/w,0);
          }else if(d==3){
            translate(-anV*500/w,0);
          }
        }
        if(t==1) image(pos[teamDir[team[getP(x,y)]]],x*500/w,y*500/h,500/w+1,500/h+1);
        if(t==-1) image(neg[teamDir[team[getP(x,y)]]],x*500/w,y*500/h,500/w+1,500/h+1);
        if(highTeam != moveTeam){
        if(t!=0&&highTeam != -1 && team[getP(x,y)]==highTeam){
          if(highMove) fill(255,255,0,128);
          else fill(0,0,0,128);
          rect(x*500/w,y*500/h,500/w+1,500/h+1);
        }
        }
        if(moveTeam != -1 && moveTeam == team[getP(x,y)]){
          popMatrix();
        }
      }
    }
    for(int x = 0;x < w;++x){
  for(int y = 0;y< h;++y){
    if(moveTeam != -1 && moveTeam == team[getP(x,y)]){
      int t = type[getP(x,y)];
      int d = teamDir[moveTeam];
              fill(255*min(2-2*anV,1),255*min(2-2*anV,1),255*(1-anV));
          if(d==0){
            if(type[getP(x,y+1)]==-t&&team[getP(x,y+1)]!=moveTeam) rect(x*500/w,(y+1)*500/h,500/w,anV*500/h);
          }else if(d==1){
            if(type[getP(x,y-1)]==-t&&team[getP(x,y-1)]!=moveTeam) rect(x*500/w,(y-anV)*500/h,500/w,anV*500/h);
          }else if(d==2){
            if(type[getP(x+1,y)]==-t&&team[getP(x+1,y)]!=moveTeam) rect((x+1)*500/w,y*500/h,anV*500/w,500/h);
          }else if(d==3){
            if(type[getP(x-1,y)]==-t&&team[getP(x-1,y)]!=moveTeam) rect((x-anV)*500/w,y*500/h,anV*500/w,500/h);
          }
    }
  }
}
    strokeWeight(4);
    stroke(255);
    for(int x = 0;x < w;++x){
      for(int y = 0;y < h;++y){
        if(x==0){
          if(fixT(team[x+y*w])!=-1) line(x*500/w,y*500/h,x*500/w,(y+1)*500/h);
        }else if(fixT(team[x+y*w]) != fixT(team[x+y*w-1])){
          line(x*500/w,y*500/h,x*500/w,(y+1)*500/h);
        }
        if(y==0){
          if(fixT(team[x+y*w])!=-1) line(x*500/w,y*500/h,(x+1)*500/w,y*500/h);
        }else if(fixT(team[x+y*w]) != fixT(team[x+y*w-w])){
          line(x*500/w,y*500/h,(x+1)*500/w,y*500/h);
        }
        if(moveTeam != -1 && moveTeam == team[getP(x,y)]){
          int d = teamDir[team[getP(x,y)]];
          pushMatrix();
          if(d==0){
            translate(0,anV*500/h);
          }else if(d==1){
            translate(0,-anV*500/h);
          }else if(d==2){
            translate(anV*500/w,0);
          }else if(d==3){
            translate(-anV*500/w,0);
          }
          if(x==0){
          line(x*500/w,y*500/h,x*500/w,(y+1)*500/h);
        }else if(team[x+y*w-1] != moveTeam){
          line(x*500/w,y*500/h,x*500/w,(y+1)*500/h);
        }
        if(y==0){
          line(x*500/w,y*500/h,(x+1)*500/w,y*500/h);
        }else if(moveTeam != team[x+y*w-w]){
          line(x*500/w,y*500/h,(x+1)*500/w,y*500/h);
        }
                  if(x==w-1){
          line((x+1)*500/w,y*500/h,(x+1)*500/w,(y+1)*500/h);
        }else if(team[x+y*w+1] != moveTeam){
          line((x+1)*500/w,y*500/h,(x+1)*500/w,(y+1)*500/h);
        }
        if(y==h-1){
          line(x*500/w,(y+1)*500/h,(x+1)*500/w,(y+1)*500/h);
        }else if(moveTeam != team[x+y*w+w]){
          line(x*500/w,(y+1)*500/h,(x+1)*500/w,(y+1)*500/h);
        }
        popMatrix();
        }
      }
    }
    for(int x = 0;x < w;++x){
      if(fixT(team[x+(h-1)*w])!=-1) line(x*500/w,500,(x+1)*500/w,500);
    }
    for(int y = 0;y < h;++y){
      if(fixT(team[w-1+y*w])!=-1) line(500,y*500/h,500,(y+1)*500/h);
    }
            strokeWeight(4);
    stroke(0,255,0,64);
    line(x1*500/w,y1*500/h,x2*500/w,y1*500/h);
    line(x1*500/w,y1*500/h,x1*500/w,y2*500/h);
    line(x2*500/w,y1*500/h,x2*500/w,y2*500/h);
    line(x1*500/w,y2*500/h,x2*500/w,y2*500/h);

  }
};
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Unst" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
