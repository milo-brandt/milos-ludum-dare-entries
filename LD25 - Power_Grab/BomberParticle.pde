class BomberParticle implements Particle{
  public float x,y,vx,vy,age;
  BomberParticle(float x,float y){
    this.x = x;
    this.y = y;
    float ang = random(TWO_PI);
    this.vx = cos(ang)*2;
    this.vy = sin(ang)*2;
    this.age = 20;
  }
  boolean render(){
    x += vx;
    y += vy;
    age--;
    noSmooth();
    stroke(255,255,0,25.5*age);
    point(x,y);
    smooth();
    return age <= 0;
  }
}
