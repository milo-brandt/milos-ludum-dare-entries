public abstract class Enemy{
   public float x,y;
   public float rad; //Radius
   public float health;
   public boolean friendly;
   Enemy(float x,float y,float rad,float health){
     this.x = x;
     this.y = y;
     this.rad = rad;
     this.health = health;
     this.friendly = false;
   }
      Enemy(float x,float y,float rad,float health,boolean friendly){
     this.x = x;
     this.y = y;
     this.rad = rad;
     this.health = health;
     this.friendly= friendly;
   }
   public abstract void update(GameStage g);
   public abstract void render();
   public void spawnDrop(GameStage g){}
}

