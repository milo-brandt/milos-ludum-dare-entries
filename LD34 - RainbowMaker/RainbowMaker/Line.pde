class Line implements Physical{
  public vector2 a;
  public vector2 b;
  public vector2 norm;
  public double normV;
  public vector2 dif;
  public double min;
  public double max;
  Line(vector2 a,vector2 b){ this.a=a;this.b=b;this.norm=a.sub(b).rot().unit();this.normV=norm.dot(a);this.dif=a.sub(b);this.min=Math.min(dif.dot(a),dif.dot(b));this.max=Math.max(dif.dot(a),dif.dot(b));}
  CollisionData checkCol(PlayerData p){
    double towards = p.veloc.dot(norm);
    double now = p.pos.dot(norm)-normV;
    if(towards == 0) return null;
    boolean sign = true;
    if(now < 0){
      towards = -towards;
      now = -now;
      sign = false;
    }
    if(towards >= 0) return null;
    double t = (now-p.rad)/-towards;
    if(t < 0) return null;
    if(t > 1) return null;
    vector2 k = p.posAt(t);
    double par = dif.dot(k);
    if(par < min || par > max)return null;
    return new CollisionData(t,p.posAt(t),sign?norm:norm.mul(-1),this);
  }
  void draw(){
       line((float)a.x,(float)a.y,(float)b.x,(float)b.y);    
  }
  vector2 getNormal(PlayerData p){
    if(p.pos.dot(norm)-normV > 0){
      return norm;
    }else{
      return norm.mul(-1);
    }
  }
  vector2 clampPosition(PlayerData p){
    double v = p.pos.dot(norm)-normV;
    if(v > 0){
      return p.pos.sub(norm.mul(v-p.rad));
    }else{
      return p.pos.sub(norm.mul(v+p.rad));
    }
  }
  boolean fallOff(PlayerData p){
    double par = dif.dot(p.pos);
    return par < min || par > max;
  }
  Physical genPath(PlayerData start,PlayerData end){
    return new Line(start.pos,end.pos);
  }

}