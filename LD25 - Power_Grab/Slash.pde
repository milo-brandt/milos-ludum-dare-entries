class Slash implements Weapon{
  public float range;
  public float angle;
  public float damage;
  public float reload;
  public float lastAttack;
  int attackAnim;
  public Slash(float range, float angle,float damage,float reload){
    this.range = range;
    this.angle = angle;
    this.damage = damage;
    this.reload = reload;
    this.lastAttack = -1000;
attackAnim = 0;
  }
  void reset(){
    lastAttack = 0;
  }
  public void attack(GameStage g){
    if(g.gameTime-lastAttack<reload) return;
    slashSound.play(0);
        attackAnim = 20;
    lastAttack=g.gameTime;
    float rs = range*range;
    float x = g.plyr.x;
    float y = g.plyr.y;
     float wdx = g.plyr.dx;
     float wdy = g.plyr.dy;
    float angC = cos(angle);
    for(int i = 0;i < g.enemies.size();i++){
      Enemy e = g.enemies.get(i);
      if(e.friendly) continue;
      float dx = e.x-x;
      float dy = e.y-y;
      float distSq = dx*dx+dy*dy;
      if(distSq<rs){
        //Dot product the displacement vector with direction vector.
        float dotP = dx*wdx+dy*wdy;
        if(dotP > angC*sqrt(distSq) ){
          e.health -= damage;
        }
      }
    }
    g.particles.add(new SlashGraphic(x,y,wdx,wdy,range,angle));
  }
  int getCoef(){
    if(attackAnim>=19) return 0;
    if(attackAnim>0) return -1;
    if(attackAnim==0) return 1;
    return 1;
  }
    public void render(Player plyr){
          fill(255);
          pushMatrix();
    translate(plyr.x,plyr.y);
    rotate(atan2(plyr.dy,plyr.dx)+HALF_PI+getCoef()*angle);
    attackAnim--;
    image(swordImage,-15,-range,30,range-6);
    popMatrix();
    }

}

