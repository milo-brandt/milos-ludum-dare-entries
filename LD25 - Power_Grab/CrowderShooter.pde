public class CrowderShooter extends Enemy{
  public float speed;
  public float bulletFreq;
  public float bulletDam;
  public float bulletSpd;
  public float lastShot;
  public boolean hit;
  public float dx,dy;
  CrowderShooter(float x, float y, float rad, float health, float speed,float bulletFreq,float bulletDam,float bulletSpd){
    super(x,y,rad,health);
    this.speed = speed;
    this.bulletFreq = bulletFreq;
    this.bulletDam = bulletDam;
    this.bulletSpd = bulletSpd;
    this.lastShot = 0;
    this.hit = false;
    this.dx = 0;
    this.dy = 1;
  }
  public void update(GameStage g){
     dx = g.plyr.x-x;
     dy = g.plyr.y-y;
    float magn = sqrt(dx*dx+dy*dy);
    if(magn < rad+g.plyr.rad+speed*g.dt){
      health = 0;
      g.plyr.health -= health;
      hit = true;
      //Some sort of "splat"
    }else{
      x += dx/magn*speed*g.dt;
      y += dy/magn*speed*g.dt;
    }
    if(g.gameTime-lastShot>bulletFreq){
      lastShot = g.gameTime;
     g.enemies.add(new EnemyBullet(this,g,bulletDam,bulletSpd)); 
    }
  }
  void render(){
      fill(255);
          pushMatrix();
    translate(x,y);
    rotate(atan2(dy,dx)+HALF_PI);
    image(tankImage,-rad,-rad,2*rad,2*rad);
    popMatrix();
  }
  void spawnDrop(GameStage g){
          boom.play(0);

    g.score += 20;
    if(!hit)
  if(random(1)>0.7)
      g.drops.add(new ChestDrop(x,y));

  }
}

