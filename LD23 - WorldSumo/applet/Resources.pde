class Resources{
  PFont score;
  
  PImage[] playerAnimation;
  
  PImage[] planets;
  
  PImage still;
  PImage asteroidHealthy;
  PImage asteroidMedium;
  PImage asteroidLow;
  
  PImage cracks;
  PImage cracks2;
  
  PImage shadow;

  public Resources(){
    still = loadImage("astrostop.png");
    score = loadFont("HPFont.vlw");
    planets = new PImage[4];
    planets[0] = loadImage("planet1.png");
    planets[1] = loadImage("planet2.png");
    planets[2] = loadImage("planet3.png");
    planets[3] = loadImage("planet4.png");

    playerAnimation = new PImage[2];
    playerAnimation[0] = loadImage("astrorun2.png");
    playerAnimation[1] = loadImage("astrorun.png");

    cracks = loadImage("cracks.png");
    cracks2 = loadImage("crackdoom.png");
    shadow = loadImage("shadow.png");
  }
};
