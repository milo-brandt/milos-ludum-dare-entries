abstract class Artifact{
  String name;
  String desc;
  Artifact(String name,String desc){
    this.name = name;
    this.desc = desc;
  }
  abstract void effect(UpgradeStage u);
}
