//PUSHBUTTON
#define buttonUP 2
#define buttonDOWN 1
#define buttonNOCHANGE 3

class PushButton{
  private:
   int pbpin;
   bool lastState;
   unsigned long debounce;
   unsigned long stateChangedTimeStamp;
   bool debounceWait;
  public:
  PushButton(int PB_PIN, unsigned long time1=100){
    pbpin=PB_PIN;
    lastState=false;
    debounce=time1;
    stateChangedTimeStamp=0;
    debounceWait=false;
  }
  void init(){
    pinMode(pbpin, INPUT_PULLUP);
    //update();
  }
  bool update(){
    static int ppb = HIGH; // the previous state of the button
    static unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
    static bool counting = false;
    bool r = false;
    int pb = digitalRead(pbpin);
    if (counting) {
      if (millis() - lastDebounceTime > debounce) { // has been waiting long enough
        r = (ppb == HIGH && pb == LOW);
        counting = false;
        ppb = pb;
    }
  }else { // !counting
    if (ppb == HIGH && pb == LOW) {
      counting = true;
      lastDebounceTime = millis(); // record the time
  }else
    ppb = pb;
  }
  return r;
}
  bool isPressed(){
    return digitalRead(pbpin)==LOW;
  }
  byte stateChanged(){
    byte r=buttonNOCHANGE;
    bool currButtonState=isPressed();
    if(debounceWait){
      if(millis()-stateChangedTimeStamp>debounce){
//        if(lastState==LOW&&currButtonState==LOW){          
//          r=buttonDOWN;
//        }else if(lastState==HIGH&&currButtonState==HIGH){
//          r=buttonUP;
//        }
        if (lastState ^ currButtonState) // is last recorded previous state different from current
          r = (lastState<<1) | currButtonState;
          debounceWait=false;
          lastState=currButtonState;
      }
    }else{
      if(lastState!=currButtonState){
        debounceWait=true;
        stateChangedTimeStamp=millis();
      }
    }
    return r;
  }
};
