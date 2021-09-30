abstract class Drop{
  public float x,y,life;
  Drop(float x,float y){
    this.x = x;
    this.y = y;
    this.life = 200;
  }
  public abstract void execute(GameStage g);
  public abstract void render();
}
