class CollisionData{
  public double timeTo;
  public vector2 pos;
  public vector2 normal;
  public Physical what;
  CollisionData(double timeTo,vector2 pos,vector2 normal,Physical what){
    this.timeTo = timeTo;
    this.pos = pos;
    this.normal = normal;
    this.what = what;
  }
}