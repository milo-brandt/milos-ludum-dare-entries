class Slime extends Enemy{
  float lifeLeft;
  Slime(Slimer s){
    super(s.x,s.y,10,10,true);
    lifeLeft = 300;
  }
  void update(GameStage g){
    lifeLeft -= g.dt;
    if(sq(x-g.plyr.x)+sq(y-g.plyr.y)<sq(rad+g.plyr.rad)){
      g.plyr.slimeTime = 120;
      health = 0;
      slimeSound.play(0);
    }
    if(lifeLeft < 0) health = 0;
  }
  void render(){
    image(slimeImage,x-10,y-10,20,20);
  }
}
