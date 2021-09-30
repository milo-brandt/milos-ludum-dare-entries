class ChestDrop extends Drop{
  ChestDrop(float x,float y){
    super(x,y);
  }
    public void execute(GameStage g){
      g.score += 50;
      g.stuffFound.add(g.getArtifact());
           chestDrop.play(0);

      //Maybe a particle too?
    }
  public void render(){

    image(chestImage,x-10,y-10,20,20);
  }
}
