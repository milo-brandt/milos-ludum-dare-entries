class PlanetBoom{
  public Planet me;
  PlanetBoom(Planet me){
    this.me = me;
  }
  boolean render(){
    me.maxRadius -= 2;
    if(me.maxRadius <= 0){
      return true;
    }
    me.render(r);
    return false;
  }
};
