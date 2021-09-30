class Planet{
  public float maxRadius;
  public Vector2 pos;
  public Vector2 veloc;
  public float rot;
  public float rotVeloc;
  public float expectedImpact;
  public float health;
  public float mass;
  public int imgNum;
  public Planet(float maxRadius,Vector2 pos,float rot,float rotVeloc,Vector2 veloc,float expectedImpact){
    this.maxRadius = maxRadius;
    this.pos = pos;
    this.rot = rot;
    this.rotVeloc = rotVeloc;
    this.veloc = veloc;
    this.expectedImpact = expectedImpact;
    this.mass = maxRadius*maxRadius*maxRadius;
    this.health = 200;
    this.imgNum = int(random(0,4));
  }
  public void render(Resources r){
    pushMatrix();
    translate(pos.x,pos.y,0);
    pushMatrix();
    rotateZ(rot);
    image(r.planets[imgNum],-maxRadius,-maxRadius,maxRadius*2,maxRadius*2);
    if(health < 50){
      image(r.cracks2,-maxRadius,-maxRadius,maxRadius*2,maxRadius*2);
    }else if(health < 150){
      image(r.cracks,-maxRadius,-maxRadius,maxRadius*2,maxRadius*2);
    }
    popMatrix();
    image(r.shadow,-maxRadius,-maxRadius,maxRadius*2,maxRadius*2);
    popMatrix();
  }
  public void update(){
    pos = Vector2.sum(pos,veloc);
    rot += rotVeloc;
  }
  public void capture(){
    rotVeloc = 0;
    veloc = new Vector2(0,0);
  }
  public boolean collideWith(Planet b){
    Vector2 dif = Vector2.dif(b.pos,pos);
    float magnitude = dif.magnitude();
    float magAccept = b.maxRadius + maxRadius;
    if(magnitude < magAccept){
      Vector2 velocDif = Vector2.dif(b.veloc,veloc);
      float colVeloc = Vector2.dotProduct(velocDif,dif)/dif.magnitude();
      if(colVeloc > 0) return false;
      float elasticity = 1;
      float youPortion = mass/(mass + b.mass);
      float mePortion = 1 - youPortion;
      colVeloc *= elasticity;
      //Vector2 accme = dif.unit(colVeloc*mePortion);
      //Vector2 accyou = dif.unit(-colVeloc*youPortion);
      //print(accme.x + "," + accme.y + ":" + accyou.x + "," + accyou.y);
      b.veloc = Vector2.sum(b.veloc,dif.unit(-colVeloc*youPortion));
      veloc = Vector2.sum(veloc,dif.unit(colVeloc*mePortion));
      b.pos = Vector2.sum(pos,dif.unit(magAccept));
      health -= mePortion*40;
      b.health -= youPortion*40;
return true;
    }
    return false;
  }
}
