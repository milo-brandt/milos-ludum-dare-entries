String levelInfos[] = {"Arrow keys to move, mouse to aim, click to attack."};
Artifact levelRequired[][] = {{},{new NormalSword()},{new MachineGun()},{new Diamond()}};
Artifact levelPossible[][] = {{new Diamond()},{new Diamond(),new MachineGun()},{new FractalGunA(),new MachineGun(),new NormalSword()},{new FractalGunA(),new MachineGun(),new NormalSword()}};
class GameStage{
  Player plyr;
  ArrayList<Enemy> enemies;
  ArrayList<Enemy> enemiesToSpawn;
  ArrayList<Enemy> enemiesToSpawnNextRound;
ArrayList<Drop> drops;
ArrayList<Particle> particles;
ArrayList<Artifact> stuffFound;
  float dt;
  float gameTime;
  float spawnTime;
  int levelNum;
  int lastTime;
  float usualMil;
  boolean done;
  boolean dead;
  int score;
  boolean newShot = false;
  float lastPlyrHealth;
  Artifact getArtifact(){
        int spawnFrom = min(levelRequired.length-1,levelNum);
    return levelPossible[spawnFrom][floor(random(levelPossible[spawnFrom].length))];
  }
  GameStage(){
    enemies = new ArrayList<Enemy>();
    enemiesToSpawn = new ArrayList<Enemy>();
    particles = new ArrayList<Particle>();
    drops = new ArrayList<Drop>();
    stuffFound = new ArrayList<Artifact>();
    dt=1;
    gameTime = 0;
    spawnTime = 600; //40*15 frames = 1/4 minute
    levelNum = 0;
    lastTime = millis();
    usualMil = 1000/40;
    dead = false;
    done = false;
    score = 0;
  }
  UpgradeStage go(){
    int spawnFrom = min(levelRequired.length-1,levelNum+1);
    UpgradeStage next = new UpgradeStage();
    next.levelNum = levelNum+1;
    next.plyr = plyr;
    next.lastToSpawn = enemiesToSpawnNextRound;
    next.score = score + 1000;
    next.purchases = stuffFound;
plyr.reset();
for(int i = 0;i < levelRequired[spawnFrom].length;i++){
  next.purchases.add(levelRequired[spawnFrom][i]);
}
/*for(int i = 0;i < levelNum+1;i++){
  next.purchases.add(levelPossible[spawnFrom][floor(random(levelPossible[spawnFrom].length))]);
}*/
    return next;
  }
  public void update(){
    int nowTime = millis();
    dt = min((nowTime-lastTime)/usualMil,40);
    lastTime = nowTime;
    gameTime += dt;
    plyr.update(this);
    for(int i = 0;i < enemies.size();i++){
      Enemy e = enemies.get(i);
      if(e.health <= 0){
        e.spawnDrop(this);
        enemies.remove(i);
        --i;
        continue;
      }
      e.update(this);
      if(e.health <= 0){
                e.spawnDrop(this);
        enemies.remove(i);
        --i;
        continue;
      }
      if(e.x > width+2*e.rad || e.x < -2*e.rad || e.y < -2*e.rad || e.y > height+2*e.rad){
        enemies.remove(i);
        --i;
        continue;
      }
    }
    float spawnP = 1;
    if(gameTime < spawnTime){
      spawnP = dt/(spawnTime-gameTime);
    }
    for(int i = 0;i < enemiesToSpawn.size();i++){
      if(random(1)<spawnP){
        enemies.add(enemiesToSpawn.get(i));
        enemiesToSpawn.remove(i);
        --i;
      }
    }
    if(enemiesToSpawn.size() == 0 && enemies.size() == 0 && drops.size() == 0){
       done = true;
    }
    if(plyr.health <= 0){
      done = true;
      dead = true;
     }
  }
  public void render(){
    if(plyr.health != lastPlyrHealth){
      lastPlyrHealth = plyr.health;
      hurtSound.play(0);
    }
        if((!gun.isPlaying() || gun.position() >= gun.length()) && newShot){ 
          gun.play(0);
          newShot = false;
        }

    background(0);
    fill(255);
              textSize(20);
          textAlign(CENTER,BOTTOM);
        if(levelNum < levelInfos.length){

          text(levelInfos[levelNum],width/2,height-5);
        }else{
        text("Score: " + score,width/2,height-5);
        }
    plyr.render();
    fill(255,255,0);
    for(int i = 0;i < drops.size();i++){
      Drop v = drops.get(i);
      v.life -= dt;
      if(v.life > 60 || (v.life % 10)<7){
        v.render();
      }
      if(v.life <= 0){
        drops.remove(i);
        i--;
        continue;
      }
      if(sq(v.x-plyr.x)+sq(v.y-plyr.y)<sq(3+plyr.rad)){
        v.execute(this);
        drops.remove(i);
        i--;
        continue;
      }
      if(sq(v.x-plyr.x)+sq(v.y-plyr.y)<sq(50+plyr.rad)){
        v.x = v.x*.9+plyr.x*.1;
        v.y = v.y*.9+plyr.y*.1;
      }
    }
    for(int i = 0;i < enemies.size();i++){
      enemies.get(i).render();
    }
    for(int i = 0;i < particles.size();i++){
      if(particles.get(i).render()){
        particles.remove(i);
        i--;
        continue;
      }
    }
    noFill();
    stroke(255);
    rect(width-5-plyr.maxHealth*2,5,2*plyr.maxHealth,20);
    fill(255);
    rect(width-5-plyr.maxHealth*2,5,plyr.health*2,20);
  }
}

