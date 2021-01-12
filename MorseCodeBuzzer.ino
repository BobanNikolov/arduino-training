#define led 12      // blink an led on output 12
#define buz 8      // output audio on pin 8
#define note 1400      // music note/pitch
#define element 50
#define dot1 element
#define dash1 (element * 3)
#define interchardelay (element * 3)
#define interworddelay (element * 7)
String code = "";
int len = 0;
char ch;
char new_char;
void dot() {
    Serial.print(".");
    digitalWrite(led, HIGH);
    tone(buz,note);
    delay(dot1);
    digitalWrite(led, LOW);
   noTone(buz);
    delay(interchardelay);
}
void dash() {
    Serial.print("-");
    digitalWrite(led, HIGH);
    tone(buz,note);
    delay(dash1);
    digitalWrite(led, LOW);
    noTone(buz);
    delay(interchardelay);
}
void A() {
    dot();
    dash();
}
void B() {
    dash();
    dot();
    dot();
    dot();
}
void C() {
    dash();
    dot();
    dash();
    dot();
}
void D() {
    dash();
    dot();
    dot();
}
void E() {
    dot();
}
void f() {
    dot();
    dot();
    dash();
    dot();
}
void G() {
    dash();
    dash();
    dot();
}
void H() {
    dot();
    dot();
    dot();
    dot();
}
void I() {
    dot();
    dot();
}
void J() {
    dot();
    dash();
    dash();
    dash();
}
void K() {
    dash();
    dot();
    dash();
}
void L() {
    dot();
    dash();
    dot();
    dot();
}
void M() {
    dash();
    dash();
}
void N() {
    dash();
    dot();
}
void O() {
    dash();
    dash();
    dash();
}
void P() {
    dot();
    dash();
    dash();
    dot();
}
void Q() {
    dash();
    dash();
    dot();
    dash();
}
void R() {
    dot();
    dash();
    dot();
}
void S() {
    dot();
    dot();
    dot();
}
void T() {
    dash();
}
void U() {
    dot();
    dot();
    dash();
}
void V() {
    dot();
    dot();
    dot();
    dash();
}
void W() {
    dot();
    dash();
    dash();
}
void X() {
    dash();
    dot();
    dot();
    dash();
}
void Y() {
    dash();
    dot();
    dash();
    dash();
}
void Z() {
    dash();
    dash();
    dot();
    dot();
}
void zero() {
    dash();
    dash();
    dash();
    dash();
    dash();
}
void one() {
    dot();
    dash();
    dash();
    dash();
    dash();
}
void two() {
    dot();
    dot();
    dash();
    dash();
    dash();
}
void three() {
    dot();
    dot();
    dot();
    dash();
    dash();
}
void four() {
    dot();
    dot();
    dot();
    dot();
    dash();
}
void five() {
    dot();
    dot();
    dot();
    dot();
    dot();
}
void six() {
    dash();
    dot();
    dot();
    dot();
    dot();
}
void seven() {
    dash();
    dash();
    dot();
    dot();
    dot();
}
void eight() {
    dash();
    dash();
    dash();
    dot();
    dot();
}
void nine() {
    dash();
    dash();
    dash();
    dash();
    dot();
}
void exclamation(){
  dash();
  dot();
  dash();
  dot();
  dash();
  dash();
}
void quotes(){
  dot();
  dash();
  dot();
  dot();
  dash();
  dot();
}
void dollar(){
  dot();
  dash();
  dot();
  dot();
  dot();
}
void quote(){
  dot();
  dash();
  dash();
  dash();
  dash();
  dot();
}
void leftbracket(){
  dash();
  dot();
  dash();
  dash();
  dot();
}
void rightbracket(){
  dash();
  dot();
  dash();
  dash();
  dot();
  dash();
}
void plus(){
  dot();
  dash();
  dot();
  dash();
  dot();
}
void comma(){
  dash();
  dash();
  dot();
  dot();
  dash();
  dash();
}
void minus(){
  dash();
  dot();
  dot();
  dot();
  dot();
  dash();
}
void point(){
  dot();
  dash();
  dot();
  dash();
  dot();
  dash();
}
void slash(){
  dash();
  dot();
  dot();
  dash();
  dot();
}
void colon(){
  dash();
  dash();
  dash();
  dot();
  dot();
  dot();
}
void semicolon(){
  dash();
  dot();
  dash();
  dot();
  dash();
  dot();
}
void equal(){
  dash();
  dot();
  dot();
  dot();
  dash();
}
void question(){
  dot();
  dot();
  dash();
  dash();
  dot();
  dot();
}
void monkey(){
  dot();
  dash();
  dash();
  dot();
  dash();
  dot();
}
void startsignal(){
  dot();
  dash();
  dot();
  dash();
  dot();
}
void endofwork(){
  dash();
  dash();
  dash();
  dot();
  dash();
  dash();
}
void morse() {
    if (ch=='!'){
      exclamation();
      Serial.print(" ");
    }else if (ch=='"'){
      quotes();
      Serial.print(" ");
    }else if (ch=='$'){
      dollar();
      Serial.print(" ");
    }else if (ch =='\''){
      quote();
      Serial.print(" ");
    }else if(ch=='('){
      leftbracket();
      Serial.print(" ");
    }else if(ch==')'){
      rightbracket();
      Serial.print(" ");
    }else if (ch=='+'){
      plus();
      Serial.print(" ");
    }else if(ch==','){
      comma();
      Serial.print(" ");
    }else if(ch=='-'){
      minus();
      Serial.print(" ");
    }else if(ch=='.'){
      point();
      Serial.print(" ");
    }else if (ch=='/'){
      slash();
      Serial.print(" ");
    }else if(ch==':'){
      colon();
      Serial.print(" ");
    }else if(ch==';'){
      semicolon();
      Serial.print(" ");
    }else if(ch=='='){
      equal();
      Serial.print(" ");
    }else if(ch=='?'){
      question();
      Serial.print(" ");
    }else if(ch=='@'){
      monkey();
      Serial.print(" ");
    }else if(ch=='['){
      startsignal();
      Serial.print(" ");
    }else if(ch==']'){
      endofwork();
      Serial.print(" ");
    }else if (ch == 'A' || ch == 'a') {
        A();
        Serial.print(" ");
    } else if (ch == 'B' || ch == 'b') {
        B();
        Serial.print(" ");
    } else if (ch == 'C' || ch == 'c') {
        C();
        Serial.print(" ");
    } else if (ch == 'D' || ch == 'd') {
        D();
        Serial.print(" ");
    } else if (ch == 'E' || ch == 'e') {
        E();
        Serial.print(" ");
    } else if (ch == 'F' || ch == 'f') {
        f();
        Serial.print(" ");
    } else if (ch == 'G' || ch == 'g') {
        G();
        Serial.print(" ");
    } else if (ch == 'H' || ch == 'h') {
        H();
        Serial.print(" ");
    } else if (ch == 'I' || ch == 'i') {
        I();
        Serial.print(" ");
    } else if (ch == 'J' || ch == 'j') {
        J();
        Serial.print(" ");
    } else if (ch == 'K' || ch == 'k') {
        K();
        Serial.print(" ");
    } else if (ch == 'L' || ch == 'l') {
        L();
        Serial.print(" ");
    } else if (ch == 'M' || ch == 'm') {
        M();
        Serial.print(" ");
    } else if (ch == 'N' || ch == 'n') {
        N();
        Serial.print(" ");
    } else if (ch == 'O' || ch == 'o') {
        O();
        Serial.print(" ");
    } else if (ch == 'P' || ch == 'p') {
        P();
        Serial.print(" ");
    } else if (ch == 'Q' || ch == 'q') {
        Q();
        Serial.print(" ");
    } else if (ch == 'R' || ch == 'r') {
        R();
        Serial.print(" ");
    } else if (ch == 'S' || ch == 's') {
        S();
        Serial.print(" ");
    } else if (ch == 'T' || ch == 't') {
        T();
        Serial.print(" ");
    } else if (ch == 'U' || ch == 'u') {
        U();
        Serial.print(" ");
    } else if (ch == 'V' || ch == 'v') {
        V();
        Serial.print(" ");
    } else if (ch == 'W' || ch == 'w') {
        W();
        Serial.print(" ");
    } else if (ch == 'X' || ch == 'x') {
        X();
        Serial.print(" ");
    } else if (ch == 'Y' || ch == 'y') {
        Y();
        Serial.print(" ");
    } else if (ch == 'Z' || ch == 'z') {
        Z();
        Serial.print(" ");
    } else if (ch == '0') {
        zero();
        Serial.print(" ");
    } else if (ch == '1') {
        one();
        Serial.print(" ");
    } else if (ch == '2') {
        two();
        Serial.print(" ");
    } else if (ch == '3') {
        three();
        Serial.print(" ");
    } else if (ch == '4') {
        four();
        Serial.print(" ");
    } else if (ch == '5') {
        five();
        Serial.print(" ");
    } else if (ch == '6') {
        six();
        Serial.print(" ");
    } else if (ch == '7') {
        seven();
        Serial.print(" ");
    } else if (ch == '8') {
        eight();
        Serial.print(" ");
    } else if (ch == '9') {
        nine();
        Serial.print(" ");
    } else if(ch == ' ') {
        delay(interworddelay);
        Serial.print("/ ");
    } else if(ch=='\n'){
      Serial.print("\n");
    }else{
        Serial.println("Unknown symbol!");
    }
}
void String2Morse() {
    len = code.length();
    for (int i = 0; i < len; i++) {
        ch = code.charAt(i);
        morse();
    }
}
void setup() {
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    pinMode(buz, OUTPUT);
    Serial.println("I am ready...");
}
void loop() {
    while (Serial.available()) {
        code = Serial.readString();
        Serial.print(code);
        Serial.print(" = ");
        String2Morse();
        Serial.println("");
    }
    delay(1000);
}
