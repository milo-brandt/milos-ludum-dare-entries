class Circle implements Physical{
  vector2 pos;
  double rad;
  Circle(vector2 pos,double rad){this.pos = pos;this.rad=rad;}
  CollisionData checkCol(PlayerData p){
    double r = p.rad+rad;
    vector2 dif= p.pos.sub(pos);
    double c = dif.normSq()-r*r;
    double b = 2*dif.dot(p.veloc);
    double a = p.veloc.normSq();
    if(b*b-4*a*c<=0) return null;
    if(c<0) return null; //Uh oh.
    if(b>0) return null; //Too late.
    if(c+b+a > 0 && 2*a+b < 0) return null; //Won't happen for one tick.
    double t = (-b-Math.sqrt(b*b-4*a*c))/(2*a); //Black magic formula.
    vector2 k = p.posAt(t);
    return new CollisionData(t,k,k.sub(pos).unit(),this);
  }
  void draw(){}
  vector2 getNormal(PlayerData p){
    return p.pos.sub(pos).unit();
  }
  vector2 clampPosition(PlayerData p){
    return p.pos.sub(pos).unit(p.rad + rad).add(pos);
  }
  boolean fallOff(PlayerData p){
    return true;
  }
  Physical genPath(PlayerData start,PlayerData end){
    return this;
  }
}