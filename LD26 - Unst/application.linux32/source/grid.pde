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
      anV += 1.0/30;
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
