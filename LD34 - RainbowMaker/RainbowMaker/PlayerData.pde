class PlayerData{
  public double rad;
  public vector2 pos;
  public vector2 veloc;
  public double dt;
  public double absTime;
  PlayerData(double rad,vector2 pos,vector2 veloc,double dt,double absTime){
    this.rad = rad; this.pos=pos;this.veloc=veloc;this.dt = dt;this.absTime =absTime;
   }
   PlayerData(PlayerData b){ this.rad = b.rad;this.pos=b.pos;this.veloc=b.veloc;this.dt=b.dt;this.absTime=b.absTime;}
   vector2 posAt(double t){
     return pos.add(veloc.mul(t));
   }
}