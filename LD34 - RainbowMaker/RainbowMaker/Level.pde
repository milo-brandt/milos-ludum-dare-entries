import java.util.LinkedList;
class Level{
  class PhysicalHolder{
    Physical p;
    double ignoreTime;
    PhysicalHolder(Physical p,double ignoreTime){
      this.p=p;this.ignoreTime=ignoreTime;
    }
  }
  PlayerData player;
  LinkedList<PhysicalHolder> stuff;
  LinkedList<Physical> killers;
  LinkedList<Flag> flags;
  Physical on;
  PlayerData pathstart;
  BranchRenderer branch;
  PlayerRenderer playerRender;
  PGraphics stage;
  double gravity;
  Level(PlayerData player){
    this.player = player;
    stuff = new LinkedList<PhysicalHolder>();
    killers = new LinkedList<Physical>();
    flags = new LinkedList<Flag>();
    on = null;
    pathstart=player;
    gravity = 377;
    stage = createGraphics(width,height,P2D);
    stage.beginDraw();
    stage.beginShape(QUADS);
    stage.fill(12,75,140);
    stage.vertex(-1,-1);
    stage.vertex(width+1,-1);
    stage.fill(90,160,255);
    stage.vertex(width+1,height+1);
    stage.vertex(-1,height+1);
    stage.endShape();
    stage.fill(255);
    stage.rectMode(CORNERS);

    stage.stroke(0);
    stage.endDraw();
         //for(Physical x : killers) x.draw();
    branch = new BranchRenderer(100);
    branch.push(player.pos);
    playerRender = new PlayerRenderer(100);
    //playerRender.pushPos(player);
  }
  void addObject(Physical p){
    stuff.add(new PhysicalHolder(p,0));
  }
  void addFlag(vector2 v){
    flags.add(new Flag(v));
  }
  void boingy(){
    
  }
  void addRect(double x1,double y1,double x2,double y2){
    stage.noStroke();
    stage.fill(255);
    vector2 a = new vector2(x1,y1);
    vector2 b = new vector2(x1,y2);
    vector2 c = new vector2(x2,y2);
    vector2 d = new vector2(x2,y1);
    //stage.fill(255,0,0);
    stage.beginDraw();
    stage.rect((float)x1,(float)y1,(float)x2,(float)y2);
    stage.endDraw();
    addObject(new Line(a,b));
        addObject(new Line(b,c));
    addObject(new Line(c,d));
    addObject(new Line(d,a));
    addObject(new Circle(a,0));
        addObject(new Circle(b,0));
    addObject(new Circle(c,0));
    addObject(new Circle(d,0));

  }
  void changeOn(Physical next){
    if(pathstart != null){     
      if(on == null){
        if(pathstart.pos.sub(player.pos).normSq()<player.rad/9){
          stuff.add(new PhysicalHolder(new Circle(player.pos,0),.09));
        }else if(Math.abs(pathstart.pos.x - player.pos.x)<1){
          stuff.add(new PhysicalHolder(new Line(pathstart.pos,player.pos),.09));
        }else{
          double v = (player.pos.x - pathstart.pos.x)/(player.absTime - pathstart.absTime);
          double a = gravity/v/v/2;
          double b = (player.pos.y - a*player.pos.x*player.pos.x - pathstart.pos.y + a*pathstart.pos.x*pathstart.pos.x)/(player.pos.x-pathstart.pos.x);
          double c = player.pos.y - a*player.pos.x*player.pos.x - b*player.pos.x;
          stuff.add(new PhysicalHolder(new Parabola(a,b,c,player.pos.x,pathstart.pos.x),.09));
        }
      }else{ 
        stuff.add(new PhysicalHolder(on.genPath(pathstart,player),.09));
        stuff.add(new PhysicalHolder(new Circle(player.pos,0),.09));
      }
    }
    pathstart = new PlayerData(player);
    on = next;
  }
  boolean RunStep(){
    branch.push(player.pos);
    playerRender.pushPos(player);
    playerRender.pushPos(player);
    playerRender.pushPos(player);

    double dt = player.dt;
    player.absTime += dt;
    CollisionData best = null;
    for(Flag v : flags){
      if(player.pos.sub(v.pos).normSq() < (player.rad+10)*(player.rad+10)){
        flags.remove(v);
        ding.play(0);
        break; //Just in case we killed the iterator...
      }
    }
    for(Physical x : killers){ //Oh yeah, copying and pasting: Coding master.
      CollisionData n = x.checkCol(player);
      if(n != null && n.timeTo < dt) return false;
    }
    if(flags.size() == 0) return true; //We win!
    for(PhysicalHolder x : stuff){ //Oh yeah, copying and pasting: Coding master.
      if(x.p==on) continue;
      if(x.ignoreTime > 0){ x.ignoreTime -= dt;continue;}
      CollisionData n = x.p.checkCol(player);
      if(n != null && (best==null || best.timeTo > n.timeTo)) best = n; 
    }
    if(best != null && best.timeTo < dt){
      player.pos = best.pos;
      if(keyCode == DOWN && keyPressed){

        boing.trigger();
          player.veloc = player.veloc.sub(player.veloc.inDirOf(best.normal).mul(2));
          changeOn(null);
      }else if(Math.abs(best.normal.x) > .7){
        boing.trigger();

        double out = best.normal.dot(player.veloc);
        vector2 upish = best.normal.rot().signTowards(new vector2(0,-1));
        double up = upish.dot(player.veloc);
          player.veloc = upish.mul(Math.max(150,50-up)).add(best.normal.mul(-out));
        changeOn(null);
      }else if(best.normal.y < 0){
        if(player.veloc.dot(best.normal) < -400){
                  player.veloc = player.veloc.sub(player.veloc.inDirOf(best.normal).mul(2));
          changeOn(null);
        boing.trigger();

        }else{
              changeOn(best.what);
      player.veloc = best.normal.rot().signTowards(new vector2(1,0).signTowards(player.veloc)).mul(133);
        }
    }else{
        boing.trigger();

        player.veloc = player.veloc.sub(player.veloc.inDirOf(best.normal).mul(1.5));
      }
    }else{
      if(on == null){
        player.pos = player.pos.add(player.veloc.mul(dt));
        player.veloc.y += gravity*dt;
      }else if(on.fallOff(player)){
        changeOn(null);
      }else{

        player.veloc = player.veloc.orthog(on.getNormal(player));
        player.pos = player.pos.add(player.veloc.mul(dt));
        player.pos = on.clampPosition(player);
        player.veloc = on.getNormal(player).rot().signTowards(new vector2(1,0).signTowards(player.veloc)).mul(133);
        if(keyPressed && keyCode != DOWN){
        boing.trigger();

          player.veloc = player.veloc.add(on.getNormal(player).mul(250));
          changeOn(null);
        }
      }
    }
    return false;
  }
  void Draw(){
    
    
    stage.beginDraw();
    //stage.ellipse((float)(player.pos.x),(float)(player.pos.y),(float)player.rad*2,(float)player.rad*2);
    branch.renderPerm(stage);
    stage.endDraw();
    image(stage,0,0,width,height);  
        branch.renderNormal();
    fill(255);
    //ellipse((float)(player.pos.x),(float)(player.pos.y),(float)player.rad*2,(float)player.rad*2);
    playerRender.draw(player.dt);
    fill(255,0,0);
    for(Flag v : flags) v.draw((float)player.absTime);
  }
}
