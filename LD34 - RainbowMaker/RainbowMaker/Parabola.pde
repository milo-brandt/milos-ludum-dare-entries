class Parabola implements Physical{
  double a;
  double b;
  double c;
  double xmin;
  double xmax;
  double ymin;
  double ymax;
      //Equation: ax^2 + bx + c - y for min<x<max.
  Parabola(PlayerData pathstart,PlayerData player,double gravity){
    double v = (player.pos.x - pathstart.pos.x)/(player.absTime - pathstart.absTime);
    a = gravity/v/v/2;
    b = (player.pos.y - a*player.pos.x*player.pos.x - pathstart.pos.y + a*pathstart.pos.x*pathstart.pos.x)/(player.pos.x-pathstart.pos.x);
    c = player.pos.y - a*player.pos.x*player.pos.x - b*player.pos.x;
    xmin = Math.min(pathstart.pos.x,player.pos.x);
    xmax = Math.max(pathstart.pos.x,player.pos.x);
    double vert = -b/(2*a);
    double Lv = a*xmin*xmin+b*xmin+c;
    double Rv = a*xmax*xmax+b*xmax+c;
    if(vert < xmin || vert > xmax){
      ymin = Math.min(Lv,Rv);
      ymax = Math.max(Lv,Rv);
    }else{
      double vertV = a*vert*vert+b*vert+c;
      ymin = Math.min(Lv,Math.min(Rv,vertV));
      ymax = Math.max(Lv,Math.max(Rv,vertV));
    }
  }
  Parabola(double a,double b,double c,double xend1,double xend2){
    this.a=a;this.b=b;this.c=c;this.xmin=Math.min(xend1,xend2);this.xmax=Math.max(xend1,xend2);
    double vert = -b/(2*a);
    double Lv = a*xmin*xmin+b*xmin+c;
    double Rv = a*xmax*xmax+b*xmax+c;
    if(vert < xmin || vert > xmax){
      ymin = Math.min(Lv,Rv);
      ymax = Math.max(Lv,Rv);
    }else{
      double vertV = a*vert*vert+b*vert+c;
      ymin = Math.min(Lv,Math.min(Rv,vertV));
      ymax = Math.max(Lv,Math.max(Rv,vertV));
    }
  }
  //a(x+tvx)^2 + b(x+tvx) + c - (y+tvy)
  CollisionData lineCheck(double x1,double y1,double dx,double dy,boolean above){
    double cp = a*x1*x1+b*x1+c-y1;
    if(above ^ cp < 0) return null;
    double bp = 2*a*x1*dx + b*dx - dy;
    double ap = a*dx*dx;
    if(bp*bp-4*ap*cp<=0) return null; //No intersection
    if(bp*cp >= 0) return null; //Both intersections behind us.
    double root1,root2;
    if(Math.abs(ap) > 0.0001){ //Magic number. You're welcome.
      double vert = -bp/(2*ap);
      double discrim = Math.sqrt(bp*bp-4*ap*cp)/(2*ap);
      root1 = vert + discrim;
      root2 = vert - discrim;
    }else{ //Linearish. Don't care about far away root.
      root1 = -cp/bp;
      root2 = root1;
    }
    double v = -1;
    if(root1*dx+x1 < xmax && root1*dx+x1 > xmin && root1 > 0){
      v = root1;
    }
    if(root2*dx+x1 < xmax && root2*dx+x1 > xmin && root2 > 0){
      v = v==-1?root2:Math.min(v,root2);
    }
    if(v==-1) return null;
    double posX = x1+dx*v;
    double posY = y1+dy*v;
    vector2 normRaw = (new vector2(b+2*a*posX,-1)).unit();
    if(cp < 0) normRaw = normRaw.mul(-1);
    return new CollisionData(v,new vector2(posX,posY),normRaw,this);
  }
  boolean inBoundingBox(PlayerData p){
    double x1 = p.pos.x;
    double y1 = p.pos.y;
    double x2 = p.pos.x + p.dt * p.veloc.x;
    double y2 = p.pos.y + p.dt * p.veloc.y;
    double xm = xmin - p.rad;
    double ym = ymin - p.rad;
    double xM = xmax + p.rad;
    double yM = ymax + p.rad;
    return (x1 > xm || x2 > xm) && (x1 < xM || x2 < xM) && (y1 > ym || y2 > ym) && (y1 < yM || y2 < yM);
    //Actually checks if two bounding boxes hit. The player's should be small enough so that it doesn't matter.
  }
  CollisionData checkCol(PlayerData p){
    if(!inBoundingBox(p)) return null;
    //Treat things as 32-gon.
    CollisionData best = null;
    double argmin=0;
    boolean above=a*p.pos.x*p.pos.x + b*p.pos.x + c - p.pos.y < 0;
    for(double arg = 0;arg < 2*Math.PI-.01;arg += Math.PI/4){
      double xo = Math.cos(arg)*p.rad;
      double yo = Math.sin(arg)*p.rad;
      CollisionData k = lineCheck(p.pos.x+xo,p.pos.y+yo,p.veloc.x,p.veloc.y,above);
      if(k != null && k.normal.dot(p.veloc) < 0){ //exclude non-hits and hits to wrong side.
        if(best == null || k.timeTo < best.timeTo){
          best=k;
          argmin=arg;
        }
      }
    }
    if(best ==null) return null; //No hit. Probably. 1.1 fudge factor to make sure.
    //Fixed point iteration could be used to get a better answer than we do.
    double xo = Math.cos(argmin)*p.rad;
    double yo = Math.sin(argmin)*p.rad;
    vector2 norm = best.normal.sub(new vector2(xo,yo)).unit();
    return new CollisionData(best.timeTo,p.posAt(best.timeTo),norm,this);

  }
  void draw(){
     for(double z = xmin;z<xmax-1;z+=4){
        line((float)(z-4),(float)(a*(z-4)*(z-4)+b*(z-4)+c),(float)z,(float)(a*z*z+b*z+c));
      }
  }
  vector2 getNormal(PlayerData p){
    //There are less dumb ways to do this. Efficiency doesn't really matter for this method.
    double least = 1000000;
    vector2 tan = new vector2(0,0);
    double mult = (a*p.pos.x*p.pos.x+b*p.pos.x+c-p.pos.y>0)?1:-1;
    for(double arg = 0;arg < 2*Math.PI-.01;arg += Math.PI/64){
      double xn = p.pos.x+Math.cos(arg)*p.rad;
      double yn = p.pos.y+Math.sin(arg)*p.rad;
      double v = mult*(a*xn*xn+b*xn+c-yn);
      if(v < least){
        least = v;
        tan = new vector2(-Math.cos(arg),-Math.sin(arg));
      }
    }
    return tan;
  }
  vector2 clampPosition(PlayerData p){
    vector2 t = getNormal(p).mul(-1);
    double x1 = p.pos.x + t.x*p.rad;
    double y1 = p.pos.y + t.y*p.rad;
    double dx = t.x;
    double dy = t.y; //Big copy.
    double cp = a*x1*x1+b*x1+c-y1;
    double bp = 2*a*x1*dx + b*dx - dy;
    double ap = a*dx*dx;
    if(bp*bp-4*ap*cp<=0) return p.pos; //NUh oh.
    double root1,root2;
    if(Math.abs(ap) > 0.0001){ //Magic number. You're welcome.
      double vert = -bp/(2*ap);
      double discrim = Math.sqrt(bp*bp-4*ap*cp)/(2*ap);
      root1 = vert + discrim;
      root2 = vert - discrim;
    }else{ //Linearish. Don't care about far away root.
      root1 = -cp/bp;
      root2 = root1;
    }
    if(Math.abs(root1)<Math.abs(root2)){
      return p.pos.add(t.mul(root1));
    }else{
      return p.pos.add(t.mul(root2));
    }
  }
  boolean fallOff(PlayerData p){
    return p.pos.x < xmin-1 || p.pos.x > xmax+1;
  }
  Physical genPath(PlayerData start,PlayerData end){
  //Maintain same vertex, fit to points. Not actually parallel curve of parabola
  //a(x1-v)^2 + c = y1
  //a(x2-v)^2 + c = y2
  //a(x1-v)^2 - a(x2-v)^2 = y1-y2
    if(Math.abs(start.pos.x-end.pos.x) < 3){
      return new Line(start.pos,end.pos);
    }
    double vert = -b/(2*a);
    double a = (start.pos.y - end.pos.y)/((start.pos.x-vert)*(start.pos.x-vert)-(end.pos.x-vert)*(end.pos.x-vert));
    double b = (start.pos.y - a*start.pos.x*start.pos.x - end.pos.y + a*end.pos.x*end.pos.x)/(start.pos.x-end.pos.x);
    double c = start.pos.y - a*start.pos.x*start.pos.x - b*start.pos.x;
    return new Parabola(a,b,c,start.pos.x,end.pos.x);
  }
}