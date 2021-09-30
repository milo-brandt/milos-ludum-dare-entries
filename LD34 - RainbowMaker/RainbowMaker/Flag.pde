class Flag{
  public vector2 pos;
  //theta = ax + c*cos(x*r); c*r > a; c > a/r
  public float rotSpeed;
  public float rotSpeedMod;
  public float rotSpeedModFreq;
  public Flag(vector2 v){
    this.pos = v;
    this.rotSpeed = random((float)(Math.PI),(float)(3*Math.PI));
    this.rotSpeedModFreq = 2*(float)Math.PI/random(3,10);
    this.rotSpeedMod = random(1.1,2.5)*rotSpeed/rotSpeedModFreq;
  }
  void draw(float t){
    pushMatrix();
    translate((float)pos.x,(float)pos.y);
    rotate(t*rotSpeed + sin(t*rotSpeedModFreq)*rotSpeedMod);
    image(flag,-10,-10,20,20);
    popMatrix();
  }
}