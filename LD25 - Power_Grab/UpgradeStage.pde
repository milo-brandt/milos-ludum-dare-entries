/*Look elsewhere. Look elsewhere. Look elsewhere. Look elsewhere.*/

class UpgradeStage{
  Player plyr;
  ArrayList<Enemy> toSpawn;
  ArrayList<Enemy> lastToSpawn;
  ArrayList<Artifact> purchases;
  int levelNum;
  boolean canClick;
  boolean goTime;
  int score;
  boolean beenOff;
  UpgradeStage(){
    toSpawn = new ArrayList<Enemy>();
    purchases = new ArrayList<Artifact>();
    canClick = false;
    goTime = false;
    score = 0;
    beenOff = false;
  }
  GameStage go(){
    //ADD basic monsters
    
    GameStage next = new GameStage();
    next.levelNum = levelNum;
    next.plyr = plyr;
    plyr.health = plyr.maxHealth;
        next.lastPlyrHealth = plyr.health;
                          
    next.enemiesToSpawn = lastToSpawn;
    for(int i = 0;i < levelNum*3;i++){
      next.enemiesToSpawn.add(new CrowderShooter(random(-10,10+width),-10,10,levelNum*12,1+random(1),10,levelNum,8));

    }
        next.enemiesToSpawnNextRound = toSpawn;
next.score = score;
    return next;
  }
  void render(){
    if(!mousePressed) canClick = true;
    background(0);
    fill(255);
    textSize(30);
    textAlign(CENTER,TOP);
    text("Take Some Artifacts!",width/2,10);
    textSize(20);
    textAlign(LEFT,TOP);
    for(int i = 0;i < purchases.size();++i){
      Artifact e = purchases.get(i);
      text(e.name,20,60+30*i);
      if(mouseY > 55+30*i && mouseY <= 85+30*i){
        textSize(15);
        text(e.desc,20,height - 120,width-20,height-60);
        textSize(20);
              if(mousePressed && canClick){
        canClick = false;
        e.effect(this);
        purchases.remove(i);
        --i;
        buySound.play(0);
      }
      }
    }
    textAlign(RIGHT,BOTTOM);
    textSize(30);
    if(!mousePressed) beenOff = true;
    if(mouseY > height - 40 && mouseX > width - 80){
      if(mousePressed && beenOff){
      goTime = true;
      }else{
        fill(255,255,0);
      }
    }
    text("GO!",width-5,height-5);
  }
}
