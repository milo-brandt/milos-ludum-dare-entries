class StarterGun extends Artifact{
  StarterGun(){
    super("Ancient Gun","This rusty old piece of junk isn't very good, but you're not going to be able to do much without it. As with all artifacts, taking the gun will summon additional enemies 2 rounds in the future, but you can use it in the next round.");
  }
  void effect(UpgradeStage u){
    for(int i = 0;i < 5;++i){
      Vector2 v = onEdge(10);
      u.toSpawn.add(new CrowderShooter(v.x,v.y,10,10,2+random(1),40,1,8));
    }
    for(int i = 0;i < 10;++i){
      Vector2 v = onEdge(10);
      u.toSpawn.add(new Bomber(v.x,v.y,10,10,3+random(3)));
    }
    u.plyr.offense.add(new Gun(10,40));

  }
}
