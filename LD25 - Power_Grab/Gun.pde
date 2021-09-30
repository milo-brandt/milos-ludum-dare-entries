class Gun implements Weapon{
  public float damage;
  public float reload;
  public float lastAttack;
  public Gun(float damage,float reload){
    this.damage = damage;
    this.reload = reload;
    this.lastAttack = -1000;
  }
  void reset(){
    lastAttack = 0;
  }
  void attack(GameStage g){

    if(g.gameTime-lastAttack<reload) return;
            g.newShot = true;

    lastAttack=g.gameTime;
    g.enemies.add(new Bullet(g,damage,10));
  }
  public void render(Player plyr){}

}
