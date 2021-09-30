class vector2{
  public double x;
  public double y;
  vector2(double x,double y){ this.x=x;this.y=y;}
  vector2 add(vector2 b){ return new vector2(x+b.x,y+b.y);}
  vector2 sub(vector2 b){ return new vector2(x-b.x,y-b.y);}
  vector2 unit(){ return this.mul(1/this.norm());}
    vector2 unit(double a){ return this.mul(a/this.norm());}

    double norm(){ return Math.sqrt(this.dot(this));}
        double normSq(){ return this.dot(this);}

  vector2 signTowards(vector2 b){
    if(this.dot(b)>0) return this; return this.mul(-1);
  }
    vector2 signFrom(vector2 b){
    if(this.dot(b)<0) return this; return this.mul(-1);
  }
  vector2 inDirOf(vector2 norm){
    return norm.mul(this.dot(norm));
  }
  vector2 orthog(vector2 norm){
    return this.sub(norm.mul(this.dot(norm)));
  }
double dot(vector2 b){ return x*b.x+y*b.y;}
  vector2 mul(double b){ return new vector2(b*x,b*y);}
  vector2 rot(){ return new vector2(-y,x);}
}