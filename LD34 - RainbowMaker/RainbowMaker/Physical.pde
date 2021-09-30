interface Physical{
  CollisionData checkCol(PlayerData p);
  void draw();
  vector2 getNormal(PlayerData p); //Gives the normal. Don't question it.
  vector2 clampPosition(PlayerData p);
  boolean fallOff(PlayerData p);
  Physical genPath(PlayerData start,PlayerData end);
}