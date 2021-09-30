class FractalBullet extends Enemy{
  public float xveloc;
  public float yveloc;
  public float damage;
  public float birth;
  public float birthRate;
  public float splitAng;
  public int splits;
  FractalBullet(GameStage g,float damage,float speed,float birthRate,float splitAng){
    super(g.plyr.x,g.plyr.y,2,1,true);
    this.xveloc = g.plyr.dx*speed;
    this.yveloc = g.plyr.dy*speed;
    this.damage = damage;
    this.birth = g.gameTime;
    this.birthRate = birthRate;
    this.splitAng = splitAng;
    this.splits = 0;
  }
    FractalBullet(GameStage g,FractalBullet b,float xveloc,float yveloc,float damage,int splits){
    super(b.x,b.y,2,1,true);
    this.xveloc = xveloc;
    this.yveloc = yveloc;
    this.damage = damage;
    this.birth = g.gameTime;
    this.birthRate = b.birthRate+1;
    this.splitAng = b.splitAng;
    this.splits = splits;
  }
  public void update(GameStage g){
    x += xveloc*g.dt;
    y += yveloc*g.dt;
    for(int i = 0;i < g.enemies.size();i++){
      Enemy e = g.enemies.get(i);
      if(e.friendly) continue;
    float dx =e.x-x;
    float dy =e.y-y;
    if(dx*dx+dy*dy < sq(2+e.rad)){
      health = 0;
      e.health -= damage;
      damage = 0;
    }
    }
    if(g.gameTime-birth > birthRate && splits < 3){
      //Budding! A right of passage into man/bullet-hood!
      float co = cos(splitAng);
      float si = sin(splitAng);
      float xv1 = co*xveloc-si*yveloc;
      float yv1 = si*xveloc+co*yveloc;
      g.enemies.add(new FractalBullet(g,this,xv1,yv1,damage,splits+1));
      xv1 = co*xveloc+si*yveloc;
      yv1 = -si*xveloc+co*yveloc;
            g.enemies.add(new FractalBullet(g,this,xv1,yv1,damage,splits+1));

      health = 0;
    }
    if(x < 0 || x > width || y < 0 || y > height) health = 0;
  }
  public void render(){
    fill(255);
    noStroke();
    ellipse(x,y,4,4);
  }
}
