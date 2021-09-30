class HealthDrop extends Drop{
  float healthGiven;
  HealthDrop(float x,float y,float healthGiven){
    super(x,y);
    this.healthGiven = healthGiven;
  }
    public void execute(GameStage g){
      healthGain.play(0);
      g.score += 50;
      g.plyr.health += healthGiven;
      if(g.plyr.health > g.plyr.maxHealth) g.plyr.health = g.plyr.maxHealth;
      //Maybe a particle too?
    }
  public void render(){
    image(heartImage,x-10,y-10,20,20);
  }
}
