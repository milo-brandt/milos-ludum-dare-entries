class Bomber extends Crowder{
    Bomber(float x, float y, float rad, float health, float speed){
      super(x,y,rad,health,speed);
    }
    void update(GameStage g){
      super.update(g);
            if(random(1)>0.7){
        g.particles.add(new BomberParticle(x,y));
      }
    }
    void render(){

      fill(255);
          pushMatrix();
    translate(x,y);
    rotate(atan2(dy,dx)+HALF_PI);
    image(bomberCharImage,-rad,-rad,2*rad,2*rad);
    popMatrix();
    }
    void spawnDrop(GameStage g){
      boom.play(0);
      g.score += 15;
      super.spawnDrop(g);
    }
}
