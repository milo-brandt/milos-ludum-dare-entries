class Player{
  public float x,y;
  public float health;
  public float maxHealth;
  public float dx,dy;
  public float rad;
  public ArrayList<Weapon> offense;
  public float walkTime;
  public float slimeTime;
  public boolean kup,kdown,kright,kleft;
  Player(float x,float y,float health,float rad){
    this.x = x;
    this.y = y;
    this.dx = 1;
    this.dy = 0;
    this.maxHealth = health;
    this.health = health;
    this.rad = rad;
    this.offense = new ArrayList<Weapon>();
    this.slimeTime = 0;
  }
  void update(GameStage g){
    dx = mouseX-x;
    dy = mouseY-y;
    float magn = sqrt(dx*dx+dy*dy);
    dx /= magn;
    dy /= magn;
    if(slimeTime > 0)
    slimeTime -= g.dt;
    float slimeEffect = slimeTime>0?0.5:1;
    x += 5*g.dt*((kright?0:-1)+(kleft?0:1))*slimeEffect;
    y += 5*g.dt*((kdown?0:-1)+(kup?0:1))*slimeEffect;
    if(x > width-rad) x = width-rad;
    if(x < rad) x = rad;
    if(y > height-rad) y = height-rad;
    if(y < rad) y = rad;
    if(kright || kleft || kdown || kup) walkTime += g.dt;
    else walkTime = 0;
    if(mousePressed){
      for(int i = 0;i < offense.size();i++){
        offense.get(i).attack(g);
      }
    }
  }
  void render(){
    fill(255);
    pushMatrix();
    translate(x,y);
    rotate(atan2(dy,dx)+HALF_PI+sin(walkTime/5)/5);
    image(slimeTime<=0?plyrImage:slimeCharImage,-10,-10,20,20);
    popMatrix();
          for(int i = 0;i < offense.size();i++){
        offense.get(i).render(this);
      }

  }
  void reset(){
      for(int i = 0;i < offense.size();i++){
        offense.get(i).reset();
      }  
      kright=false;
      kup=false;
      kdown=false;
      kleft=false;
      health = maxHealth;
      slimeTime = 0;
    }
}

