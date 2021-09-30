class BranchRenderer{
  public vector2[] past;
  public int pos;
  public int maxPos;
  public int history;
  BranchRenderer(int max){
    past = new vector2[max];
    pos = 0;
    history = 0;
    maxPos = max;
  }
  void push(vector2 v){
    past[pos]=new vector2(v.x,v.y);
    ++pos;
    if(pos == maxPos) pos = 0;
    if(history < maxPos) ++history;
  }
  void renderNormal(){
    vector2 up=new vector2(0,-1);
    if(history < 4) return;
    noStroke();
    for(int i = 1;i<history-3;++i){
      vector2 a=past[(pos-i+maxPos)%maxPos];
      vector2 b=past[(pos-i-1+maxPos)%maxPos];
      vector2 c=past[(pos-i-2+maxPos)%maxPos];
      vector2 d=past[(pos-i-3+maxPos)%maxPos];
      vector2 bdif = a.sub(c).unit().rot().signTowards(up);
      vector2 cdif = b.sub(d).unit().rot().signTowards(up);
      float alpha = min(i*1.5,127);

      beginShape(QUAD_STRIP);
      fill(255,0,0,0);
      vertex((float)(c.x+4.5*cdif.x),(float)(c.y+4.5*cdif.y));
      vertex((float)(b.x+4.5*bdif.x),(float)(b.y+4.5*bdif.y));
      fill(255,0,0,alpha);
      vertex((float)(c.x+3*cdif.x),(float)(c.y+3*cdif.y));
      vertex((float)(b.x+3*bdif.x),(float)(b.y+3*bdif.y));
      fill(255,255,0,alpha);
      vertex((float)(c.x+1.5*cdif.x),(float)(c.y+1.5*cdif.y));
      vertex((float)(b.x+1.5*bdif.x),(float)(b.y+1.5*bdif.y));
      fill(0,255,0,alpha);
      vertex((float)(c.x+0*cdif.x),(float)(c.y+0*cdif.y));
      vertex((float)(b.x+0*bdif.x),(float)(b.y+0*bdif.y));
      fill(0,255,255,alpha);
      vertex((float)(c.x-1.5*cdif.x),(float)(c.y-1.5*cdif.y));
      vertex((float)(b.x-1.5*bdif.x),(float)(b.y-1.5*bdif.y));
       fill(0,0,255,alpha);
      vertex((float)(c.x-3*cdif.x),(float)(c.y-3*cdif.y));
      vertex((float)(b.x-3*bdif.x),(float)(b.y-3*bdif.y));     
       fill(0,0,255,0);
      vertex((float)(c.x-4.5*cdif.x),(float)(c.y-4.5*cdif.y));
      vertex((float)(b.x-4.5*bdif.x),(float)(b.y-4.5*bdif.y));    
      endShape();
    }
    /*if(history == maxPos){
    vector2 e = past[pos];
    fill(255,255,255,20);
    for(int i = 0;i<100;++i){
      ellipse((float)(random(-7,7)+e.x),(float)(random(-7,7)+e.y),5,5);
    }
    }*/
  }
  void renderPerm(PGraphics p){
    if(history<maxPos) return;
        vector2 up=new vector2(0,-1);

      vector2 a=past[(pos+3)%maxPos];
      vector2 b=past[(pos+2)%maxPos];
      vector2 c=past[(pos+1)%maxPos];
      vector2 d=past[pos];
      vector2 bdif = a.sub(c).unit(2).rot().signTowards(up);
      vector2 cdif = b.sub(d).unit(2).rot().signTowards(up);
    p.noStroke();
     float alpha=128;
     float k = .75;
p.beginShape(QUAD_STRIP);
      p.fill(255,0,0,0);
      p.vertex((float)(c.x+3*k*cdif.x),(float)(c.y+3*k*cdif.y));
      p.vertex((float)(b.x+3*k*bdif.x),(float)(b.y+3*k*bdif.y));
      p.fill(255,0,0,alpha);
      p.vertex((float)(c.x+2*k*cdif.x),(float)(c.y+2*k*cdif.y));
      p.vertex((float)(b.x+2*k*bdif.x),(float)(b.y+2*k*bdif.y));
      p.fill(255,255,0,alpha);
      p.vertex((float)(c.x+1*k*cdif.x),(float)(c.y+1*k*cdif.y));
      p.vertex((float)(b.x+1*k*bdif.x),(float)(b.y+1*k*bdif.y));
      p.fill(0,255,0,alpha);
      p.vertex((float)(c.x+0*k*cdif.x),(float)(c.y+0*k*cdif.y));
      p.vertex((float)(b.x+0*k*bdif.x),(float)(b.y+0*k*bdif.y));
      p.fill(0,255,255,alpha);
      p.vertex((float)(c.x-1*k*cdif.x),(float)(c.y-1*k*cdif.y));
      p.vertex((float)(b.x-1*k*bdif.x),(float)(b.y-1*k*bdif.y));
       p.fill(0,0,255,alpha);
      p.vertex((float)(c.x-2*k*cdif.x),(float)(c.y-2*k*cdif.y));
      p.vertex((float)(b.x-2*k*bdif.x),(float)(b.y-2*k*bdif.y));     
       p.fill(0,0,255,0);
      p.vertex((float)(c.x-3*k*cdif.x),(float)(c.y-3*k*cdif.y));
      p.vertex((float)(b.x-3*k*bdif.x),(float)(b.y-3*k*bdif.y));    
      p.endShape();
}
}