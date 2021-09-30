public class Slimer extends Enemy{
  public float speed;
  public boolean hit;
  public float lastSlime;
  public float dx,dy;
  Slimer(float x, float y, float rad, float health, float speed){
    super(x,y,rad,health);
    this.speed = speed;
    this.hit = false;
    this.lastSlime = -1000;
    this.dx = 0;
    this.dy = 1;
  }
  public void update(GameStage g){
     dx = g.plyr.x-x;
     dy = g.plyr.y-y;
    float magn = sqrt(dx*dx+dy*dy);
    if(g.gameTime - lastSlime > 65){
      lastSlime = g.gameTime;
      g.enemies.add(new Slime(this));
    }
    if(magn < rad+g.plyr.rad+speed*g.dt){
      g.plyr.health -= min(health,g.plyr.maxHealth/3);
            health = 0;
hit = true;
      //Some sort of "splat"
    }else{
      x += dx/magn*speed*g.dt;
      y += dy/magn*speed*g.dt;
    }
  }
  void render(){
      fill(255);
          pushMatrix();
    translate(x,y);
    rotate(atan2(dy,dx)+HALF_PI);
    image(slimerImage,-20,-20,40,40);
    popMatrix();
  }
  void spawnDrop(GameStage g){
    g.score += 50;
    if(!hit){
    if(random(1)>0.7)
    g.drops.add(new HealthDrop(x,y,10));
      if(random(1)>0.9)
  g.drops.add(new ChestDrop(x,y));
  }
  }
}

