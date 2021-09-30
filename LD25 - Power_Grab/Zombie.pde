class Zombie extends Crowder{
    Zombie(float x, float y, float rad, float health, float speed){
      super(x,y,rad,health,speed);
    }
    void render(){

      fill(255);
          pushMatrix();
    translate(x,y);
    rotate(atan2(dy,dx)+HALF_PI);
    image(zombieCharImage,-10,-10,20,20);
    popMatrix();
    }
   void spawnDrop(GameStage g){
     hurtSound.play(0);
    g.score += 10;
   }
}
