static String[] diamondAdjectives = {"sparkly","shining","blood soaked","expensive","fake","romantic","sharp","valuable","inedible","tiny","abnormally large","suspiciously brittle"};
String diamondDescription(){
    int x = floor(random(0,diamondAdjectives.length));
    int y = floor(random(0,diamondAdjectives.length-1));
    if(y>=x){
      y++;
    }
    return "This " + diamondAdjectives[x] + " and " + diamondAdjectives[y] + " diamond will attract some quick people who are also bombs... I bet you could use it as armor, though.";
}
class Diamond extends Artifact{
  Diamond(){
    super("Diamond",diamondDescription());
  }
  void effect(UpgradeStage u){
    u.score += 100;
    for(int i = 0;i < 5;i++){
    u.toSpawn.add(new Bomber(random(-10,width+10),-10,7,20*u.levelNum,7));
    }
    u.plyr.maxHealth += 20;
  }
}
