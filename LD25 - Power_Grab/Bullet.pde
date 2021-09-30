class Bullet extends Enemy{
  public float xveloc;
  public float yveloc;
  public float damage;
  Bullet(GameStage g,float damage,float speed){
    super(g.plyr.x,g.plyr.y,2,1,true);
    this.xveloc = g.plyr.dx*speed;
    this.yveloc = g.plyr.dy*speed;
    this.damage = damage;
  }
  public void update(GameStage g){
    x += xveloc*g.dt;
    y += yveloc*g.dt;
    for(int i = 0;i < g.enemies.size();i++){
      Enemy e = g.enemies.get(i);
      if(e.friendly) continue;
    float dx =e.x-x;
    float dy =e.y-y;
    if(dx*dx+dy*dy < sq(2+e.rad)){
      health = 0;
      e.health -= damage;
      damage = 0;
    }
    }
    if(x < 0 || x > width || y < 0 || y > height) health = 0;
  }
  public void render(){
    fill(255);
    noStroke();
    ellipse(x,y,4,4);
  }
}
