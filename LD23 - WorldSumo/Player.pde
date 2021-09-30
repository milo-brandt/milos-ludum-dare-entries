class Player{
  Vector2 pos;
  Vector2 camDown;
  Vector2 veloc;
  Planet home;
  Planet on;
  float playerRadius;
  float speed;
  float downSpeed;
  public Bounds screenBounds;
  public boolean alive;
  public boolean faceLeft;
  public boolean running;
  Player(Vector2 pos,Planet home,float playerRadius,float speed){
    this.pos = pos;
    this.home = home;
    this.on = home;
    this.camDown = home.pos;
    this.playerRadius = playerRadius;
    this.speed = speed;
    this.downSpeed = 0.5;
    this.screenBounds = new Bounds(new Vector2(0,0),new Vector2(1,0));
    this.veloc = home.veloc;
    this.alive = true;
    this.faceLeft = true;
    this.running = false;
  }
  void setCamera(){
    camDown = Vector2.sum(camDown,home.veloc);
    camDown = new Vector2(camDown.x * 0.9 + home.pos.x * 0.1,camDown.y * 0.9 + home.pos.y * 0.1);
    Vector2 u = Vector2.dif(camDown,pos).unit();
    screenBounds.c = pos;
    screenBounds.u = u;
    boolean jumping = on == null || Vector2.dif(on.pos,pos).magnitude() >= on.maxRadius + playerRadius + 3;
    camera();
    pushMatrix();
    translate(width/2+(faceLeft?10:-10),height/2-10);
    scale(faceLeft?-1:1,1,1);
    image(jumping?r.playerAnimation[1]:running?r.playerAnimation[int(totalTime/7)%2]:r.still,0,0,20,20);
    popMatrix();
    beginCamera();
    translate(pos.x-width/2,pos.y-height/2,0);
    rotateZ(atan2(u.y,u.x)-HALF_PI);
    endCamera();
  }
  Vector2 getVelocity(){
    Vector2 d = Vector2.dif(home.pos,pos);
    return d.mul(downSpeed);
  }
  void movePlyr(Input in){
    if(pos.magnitude() > maxDist){
      kill();
      return;
    }
    Vector2 d = Vector2.dif(home.pos,pos);
    d = d.unit();
    Vector2 l = d.perpCW();
    if(on != null){
    float magnitude = Vector2.dif(on.pos,pos).magnitude();
    if(magnitude >= on.maxRadius + playerRadius + 1){
      downSpeed += 0.25;
    }else if(downSpeed >= -1){
      veloc = on.veloc;
      if(in.isDown(Input.JUMP) && downSpeed > -5){
        downSpeed = -10;
      }else{
        if(magnitude >= on.maxRadius + playerRadius){
          downSpeed = 0;
        }else{
          downSpeed = -0.9;
        }
      }
    }
    }
    pos = Vector2.sum(Vector2.sum(pos,d.mul(downSpeed)),veloc);
    float lSpeed = (in.isDown(Input.MRIGHT) ? speed : 0) - (in.isDown(Input.MLEFT) ? speed : 0);
    if(lSpeed != 0 && on != null){
      running = true;
     faceLeft = lSpeed > 0;
     pos = Vector2.sum(l.mul(lSpeed),pos);
    }else{
      running = false;
    }
  }
  void switchTo(Planet b){
    if(on != b){
      downSpeed = abs(downSpeed);
      //veloc = Vector2.sum(Vector2.dif(home.pos,pos).unit(downSpeed),veloc);
      on = b;
      home = b; 
    }
  }
  boolean collideWith(Planet b){
    Vector2 dif = Vector2.dif(b.pos,pos);
    float magnitude = dif.magnitude();
    float magAccept = b.maxRadius + playerRadius;
    if(magnitude < magAccept){
      downSpeed = 0;
      pos = Vector2.sum(b.pos,dif.unit(-magAccept));
      return true;
    }
    return false;
  }
  void kill(){
    alive = false;
    println("I DIED");
  }
}
