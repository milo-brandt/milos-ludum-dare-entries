class EnemyBullet extends Enemy{
  public float xveloc;
  public float yveloc;
  public float damage;
  EnemyBullet(Enemy firer,GameStage g,float damage,float speed){
    super(firer.x,firer.y,2,1,true);
    float dx = g.plyr.x-firer.x;
    float dy = g.plyr.y-firer.y;
    float magn = sqrt(dx*dx+dy*dy);
    this.xveloc = dx/magn*speed;
    this.yveloc = dy/magn*speed;
    this.damage = damage;
  }
  public void update(GameStage g){
    x += xveloc*g.dt;
    y += yveloc*g.dt;
    float dx = g.plyr.x-x;
    float dy = g.plyr.y-y;
    if(dx*dx+dy*dy < sq(rad+g.plyr.rad)){
      health = 0;
      g.plyr.health -= damage;
    }
    if(x < 0 || x > width || y < 0 || y > height) health = 0;
  }
  public void render(){
    fill(255);
    noStroke();
    ellipse(x,y,2*rad,2*rad);
  }
}
