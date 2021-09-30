class FractalGunA extends Artifact{
  FractalGunA(){
    super("Fractal Gun","The engineers tell me this gun no longer destroys the world when used! Although, having your bullets split apart midflight isn't as useful as one would hope. Oh, and the engineers want it back. They have big guns.");
  }
  void effect(UpgradeStage u){
        u.score += 200;

    for(int i = 0;i < 3+u.levelNum/2;++i){
      Vector2 v = onEdge(15);
      u.toSpawn.add(new CrowderShooter(v.x,v.y,15,u.levelNum*20,1+random(1),5,u.levelNum,8));
    }
    u.plyr.offense.add(new FractalGun(4+random(5),35+random(10),random(PI/6),8+random(7)));

  }
}
