class Bounds{
  public Vector2 c;
  public Vector2 u;
  public Bounds(Vector2 c,Vector2 u){
    this.c = c;
    this.u = u;
  }
  Vector2 transform(Vector2 world){
    Vector2 r = Vector2.dif(world,c);
    return new Vector2(r.x*u.y - r.y*u.x,r.y*u.y + r.x*u.x); //Using the ultra-scientific method of "I can't remember which order matrix multiplication goes in, but this works"
  }
  Vector2 transformSpeed(Vector2 r){
    return new Vector2(r.x*u.y - r.y*u.x,r.y*u.y + r.x*u.x); //I have no idea. It works though.
  }
}
