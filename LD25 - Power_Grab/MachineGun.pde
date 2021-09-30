class MachineGun extends Artifact{
  MachineGun(){
    super("Machine Gun","Pew pew pew!");
  }
  void effect(UpgradeStage u){
        u.score += 100;

    for(int i = 0;i < 6+u.levelNum/2;++i){
      u.toSpawn.add(new Slimer(random(-10,width+10),-10,20,50*u.levelNum,2+random(3)));
    }
    u.plyr.offense.add(new Gun(1+random(2),2+random(2)));
  }
}
