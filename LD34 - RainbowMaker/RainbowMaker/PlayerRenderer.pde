class PlayerRenderer{
  public vector2[] past;
  public vector2[] pastV;
  public int pos;
  public int maxPos;
  public int history;
  public double t;
  void drawBlob(float alpha,float size){
    noStroke();
    beginShape(TRIANGLE_FAN);
    fill(255,255,255,alpha);
    vertex(0,0);
    fill(255,255,255,0);
    vertex(size,0);
    vertex(0,size);
    vertex(-size,0);
    vertex(0,-size);
    vertex(size,0);
    endShape();
  }
  PlayerRenderer(int max){
    past = new vector2[max];
    pastV = new vector2[max];
    pos = 0;
    history = 0;
    maxPos = max;
    t = 0;
  }
  void pushPos(PlayerData p){
    past[pos]=p.pos;
    vector2 v = p.veloc.unit();
    pastV[pos]=v.mul(random(0,50)).add(v.rot().mul(random(-150,150)));
    ++pos;
    if(pos==maxPos)pos = 0;
    if(history<maxPos) ++history;
  }
  void draw(double dt){
    t+=dt;
    for(int i = 1; i<=history;++i){
      int p = (pos - i + maxPos)%maxPos;
      float t = 1-((float)i)/maxPos;
      past[p] = past[p].add(pastV[p].mul(dt));
      pushMatrix();
      translate((float)past[p].x,(float)past[p].y);
      rotate((float)(t*10+i*1));
      drawBlob(t*t*255,12-7*(1-t));
      popMatrix();
    }
  }
}