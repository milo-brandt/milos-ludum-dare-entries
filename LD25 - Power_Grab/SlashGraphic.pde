class SlashGraphic implements Particle{
  float x,y;
  float ang1;
  float ang2;
  float rad;
  float age;
  SlashGraphic(float x,float y,float dx,float dy,float rad,float ang){
    this.x = x;
    this.y = y;
    float angMid = atan2(dy,dx);
    this.ang1 = angMid-ang;
    this.ang2 = angMid+ang;
    this.rad = rad;
    this.age = 1;
  }
  public boolean render(){
    age -= 0.2;
    noStroke();
    fill(255,255,255,255*age);
      arc(x,y,2*rad,2*rad,ang1,ang2);
    return age<=0;
  }
}
