import java.util.HashSet;
import java.util.HashMap;

class Input{
  static final int JUMP = 0;
  static final int DUCK = 1;
  static final int MRIGHT = 2;
  static final int MLEFT = 3;

  public HashMap keysToControls;
  public HashMap keyCodesToControls;
  public HashSet controlsDown;
  public Input(){
    keysToControls = new HashMap();
    keyCodesToControls = new HashMap();
    controlsDown = new HashSet();
    
    keysToControls.put(' ',JUMP);
    keysToControls.put('w',JUMP);
    keyCodesToControls.put(UP,JUMP);
    
    keysToControls.put('s',DUCK);
    keyCodesToControls.put(DOWN,DUCK);
    
    keysToControls.put('d',MRIGHT);
    keyCodesToControls.put(RIGHT,MRIGHT);
    
    keysToControls.put('a',MLEFT);
    keyCodesToControls.put(LEFT,MLEFT);
  }
  public boolean isDown(int c){
    return controlsDown.contains(c);
  }
};
