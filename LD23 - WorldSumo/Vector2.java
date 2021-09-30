class Vector2{
  public float x,y;
  public Vector2(float x,float y){
    this.x = x;
    this.y = y;
  }
  static public Vector2 angleRadius(float ang,float rad){
    return new Vector2((float)Math.sin(ang)*rad,(float)Math.cos(ang)*rad);
  }
  static public float dotProduct(Vector2 a,Vector2 b){
    return a.x*b.x + a.y*b.y;
  }
  static public Vector2 sum(Vector2 a,Vector2 b){
    return new Vector2(a.x+b.x,a.y+b.y);
  }
  static public Vector2 dif(Vector2 a,Vector2 b){
    return new Vector2(a.x-b.x,a.y-b.y);
  }
  static public float distance(Vector2 a,Vector2 b){
    return dif(a,b).magnitude();
  }
  public float magnitude(){
    return (float)Math.sqrt(x*x+y*y);
  }
  public Vector2 unit(){
    float coef = 1/magnitude();
    return new Vector2(x*coef,y*coef);
  }
  public Vector2 unit(float m){
    float coef = m/magnitude();
    return new Vector2(x*coef,y*coef);
  }
  public Vector2 perpCW(){
    return new Vector2(y,-x);
  }
  public Vector2 perpCCW(){
    return new Vector2(-y,x);
  }
  public Vector2 mul(float m){
    return new Vector2(m*x,m*y);
  }
}
