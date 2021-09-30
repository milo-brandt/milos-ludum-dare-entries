class NormalSword extends Artifact{
  NormalSword(){
    super("Super Pointy Stick","You'd be surprised how many trees we had to kill to find a stick this pointy. It's powerful at short range and can strike multiple enemies. Unfortunately, it causes enemies to come from all sides.");
  }
  void effect(UpgradeStage u){
        u.score += 150;

    for(int i = 0;i < 3+u.levelNum/2;++i){
      Vector2 v = onEdge(10);
      u.toSpawn.add(new FleeShooter(v.x,v.y,10,10*u.levelNum,1+random(1),20/u.levelNum,1,8));
    }
    for(int i = 0;i < u.levelNum;++i){
      Vector2 v = onEdge(10);
      u.toSpawn.add(new Crowder(v.x,v.y,10,10+random(10)*u.levelNum,1+random(1)));
    }
    u.plyr.offense.add(new Slash(60+random(40),PI/6+random(PI/3),30+random(20),20+random(20)));

  }
}
