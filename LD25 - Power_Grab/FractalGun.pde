class FractalGun implements Weapon{
  public float damage;
  public float reload;
  public float lastAttack;
  public float splitAng;
  public float birthRate;
  public FractalGun(float damage,float reload,float splitAng,float birthRate){
    this.damage = damage;
    this.reload = reload;
    this.lastAttack = -1000;
    this.splitAng = splitAng;
    this.birthRate = birthRate;
  }
  void reset(){
    lastAttack = 0;
  }
  void attack(GameStage g){
    if(g.gameTime-lastAttack<reload) return;
        g.newShot = true;

    lastAttack=g.gameTime;
    g.enemies.add(new FractalBullet(g,damage,10,birthRate,splitAng));
  }
  public void render(Player plyr){}

}
