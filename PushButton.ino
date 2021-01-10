//PUSHBUTTON code
#define LED_PIN1 5
#define LED_PIN2 6
#define LED_BUILTIN 13
#define buttonUP 1
#define buttonDOWN 2
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
    pinMode(pbpin, INPUT);
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
void toggleLed(){
  static bool state=false;
  if(state){
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, HIGH); 
  }else{
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, LOW); 
  }
  state=!state;
}
void toggleLed1(){
  static bool state = false;
  if(state){
    digitalWrite(LED_BUILTIN, LOW);
  }else{
    digitalWrite(LED_BUILTIN, HIGH);
  }
  state=!state;
}
int PB_PIN_MINUS=2;
int PB_PIN_START=3;
PushButton Button1(PB_PIN_MINUS,300);
PushButton Button2(PB_PIN_START, 500);
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Button1.init();
  Button2.init();
  digitalWrite(LED_PIN1, HIGH);
  digitalWrite(LED_PIN2, LOW);  
  digitalWrite(LED_BUILTIN, LOW);
  toggleLed();
  delay(500);
  toggleLed();
  delay(500);
  toggleLed();
  delay(500);

}

void loop() {
//  if(Button1.isPressed()){
//    toggleLed();  
//  }
  
  byte stateButton=Button1.stateChanged();
  Serial.println(stateButton);
  if(stateButton==buttonDOWN){
    toggleLed();
  }
  byte stateButton1=Button2.stateChanged();
  if(stateButton1==buttonDOWN){
    toggleLed1();
  }
}
