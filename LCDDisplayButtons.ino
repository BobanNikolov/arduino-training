//1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define buttonUP 2
#define buttonDOWN 1
#define buttonNOCHANGE 3
bool buttonState=false;
bool lastButtonState=false;
#include "PushButton.h"
#include "TimeClock.h"
int PB_SETUP=2;
int PB_PLUS=3;
int PB_MINUS=4;
PushButton buttonSETUP(PB_SETUP);
PushButton buttonPLUS(PB_PLUS);
PushButton buttonMINUS(PB_MINUS);
const int KEY_PRESSED=HIGH;
const unsigned long KEY_HOLD=100;
unsigned long buttonPressTime;
bool buttonDown=false;
enum clockState { UNDEFINED, RUNNING, SETSEC, SETMIN, SETHOUR } setupState;
TimeClock timeClock(12,0,0);
char timeString[16];
byte buttonStateSETUP=buttonNOCHANGE;
byte buttonStatePLUS=buttonNOCHANGE;
byte buttonStateMINUS=buttonNOCHANGE;
unsigned long int lastSetupTime=0;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void setup(){
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
  buttonSETUP.init();
  buttonPLUS.init();
  buttonMINUS.init();
  lcd.setCursor(0,0);
  lcd.print("27 Dec 2020");
  setupState=RUNNING;
  TimeClock tc(12,58,58);
  snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", tc.getHour(),tc.getMinutes(),tc.getSeconds());
  Serial.println(timeString);
  tc.addSeconds(1);
  tc.checkTime();
  snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", tc.getHour(),tc.getMinutes(),tc.getSeconds());
  Serial.println(timeString);
}

void loop()
{
  buttonStateSETUP=buttonSETUP.stateChanged();
  if(buttonStateSETUP==buttonUP){
    lastSetupTime=millis();
    Serial.println(setupState);
    if(setupState==RUNNING){
      Serial.println(setupState);
      setupState=SETHOUR;
      lcd.setCursor(0,1);
      lcd.blink();
    }else if(setupState==SETHOUR){
      Serial.println(setupState);
      lcd.setCursor(0,1);
      lcd.noBlink();
      setupState=SETMIN;
      lcd.setCursor(3,1);
      lcd.blink();
    }else if(setupState==SETMIN){
      Serial.println(setupState);
      lcd.setCursor(3,1);
      lcd.noBlink();
      setupState=SETSEC;
      lcd.setCursor(7,1);
      lcd.blink();
    }else if(setupState==SETSEC){
      Serial.println(setupState);
      lcd.setCursor(7,1);
      lcd.noBlink();
      setupState=RUNNING;
    }
  }
  buttonStatePLUS=buttonPLUS.stateChanged();
  if(buttonStatePLUS==buttonDOWN){
    lastSetupTime=millis();
    if(setupState==SETHOUR){
      Serial.println(setupState);
      timeClock.addHours(1);
      timeClock.checkTime();
      lcd.setCursor(0,1);
      snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
      //timeClock.printTime();
      Serial.println(timeString);
      lcd.print(timeString);
    }else if(setupState==SETMIN){
      Serial.println(setupState);
      timeClock.addMinutes(1);
      timeClock.checkTime();
      lcd.setCursor(0,1);
      snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
      //timeClock.printTime();
      lcd.print(timeString);
    }else if(setupState==SETSEC){
      Serial.println(setupState);
      timeClock.addSeconds(1);
      timeClock.checkTime();
      lcd.setCursor(0,1);
      snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
      //timeClock.printTime();
      lcd.print(timeString);
    }
    
  }
  buttonStateMINUS=buttonMINUS.stateChanged();
  if(buttonStateMINUS==buttonDOWN){
    lastSetupTime=millis();
    if(setupState==SETHOUR){
      Serial.println(setupState);
      timeClock.minusHours(1);
      timeClock.checkTime();
      lcd.setCursor(0,1);
      snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
      //timeClock.printTime();
      lcd.print(timeString);
      lcd.setCursor(0,1);
    }else if(setupState==SETMIN){
      Serial.println(setupState);
      timeClock.minusMinutes(1);
      timeClock.checkTime();
      lcd.setCursor(0,1);
      snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
      //timeClock.printTime();
      lcd.print(timeString);
    }else if(setupState==SETSEC){
      Serial.println(setupState);
      timeClock.minusSeconds(1);
      timeClock.checkTime();
      lcd.setCursor(0,1);
      snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
      //timeClock.printTime();
      lcd.print(timeString);
    }
    
  }
  if(millis() - lastSetupTime>=1000){
    Serial.println(setupState);
    lastSetupTime=millis();
    timeClock.addSeconds(1);
    timeClock.checkTime();
    if(setupState==RUNNING){
      lcd.setCursor(0,1);
      snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
      //timeClock.printTime();
      lcd.print(timeString);
    }
  }
//  if(buttonSETUP.stateChanged()==buttonDOWN){
//    Serial.println(setupState);
//    buttonStateSETUP=buttonSETUP.isPressed();
//    if(buttonState!=lastButtonState){
//      if(buttonState==KEY_PRESSED){
//        buttonPressTime=millis();
//        buttonDown=true;
//      }else{
//        buttonDown=false;
//      }
//      delay(50);
//    }
//    lastButtonState=buttonState;
//    lcd.setCursor(0,1);
//    snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
//    lcd.print(timeString);
//  }
//  if(buttonDown==true&&millis()-buttonPressTime>=KEY_HOLD){
//  lastSetupTime=millis();
//    Serial.println("KEY HOLD");
//    buttonDown=false;
//    if(setupState==RUNNING){
//      Serial.println(setupState);
//      setupState=SETHOUR;
//      lcd.setCursor(0,1);
//      lcd.blink();
//    }else if(setupState==SETHOUR){
//      Serial.println(setupState);
//      lcd.setCursor(0,1);
//      lcd.noBlink();
//      setupState=SETMIN;
//      lcd.setCursor(3,1);
//      lcd.blink();
//    }else if(setupState==SETMIN){
//      Serial.println(setupState);
//      lcd.setCursor(3,1);
//      lcd.noBlink();
//      setupState=SETSEC;
//      lcd.setCursor(7,1);
//      lcd.blink();
//    }else if(setupState==SETSEC){
//      Serial.println(setupState);
//      lcd.setCursor(7,1);
//      lcd.noBlink();
//      setupState=RUNNING;
//    }
//  }
//  
//  if(setupState>=2&&setupState<=4){
//    Serial.println(setupState);
//    if(setupState==SETHOUR){
//      buttonStatePLUS=buttonPLUS.stateChanged();
//      buttonStateMINUS=buttonMINUS.stateChanged();
//      if(buttonStatePLUS==buttonDOWN){
//        timeClock.addHours(1);
//        timeClock.checkTime();
//        lcd.setCursor(0,1);
//        snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
//        lcd.print(timeString);
//      }else if(buttonStateMINUS==buttonDOWN){
//        timeClock.minusHours(1);
//        timeClock.checkTime();
//        lcd.setCursor(0,1);
//        snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
//        lcd.print(timeString);
//      }
//    }else if(setupState==SETMIN){
//      buttonStatePLUS=buttonPLUS.stateChanged();
//      buttonStateMINUS=buttonMINUS.stateChanged();
//      if(buttonStatePLUS==buttonDOWN){
//        timeClock.addMinutes(1);
//        timeClock.checkTime();
//        lcd.setCursor(0,1);
//        snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
//        lcd.print(timeString);  
//      }else if(buttonStateMINUS==buttonDOWN){
//        timeClock.minusMinutes(1);
//        timeClock.checkTime();
//        lcd.setCursor(0,1);
//        snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
//        lcd.print(timeString);
//      }
//    }else if(setupState==SETSEC){
//      buttonStatePLUS=buttonPLUS.stateChanged();
//      buttonStateMINUS=buttonMINUS.stateChanged();
//      if(buttonStatePLUS==buttonDOWN){
//        timeClock.addSeconds(1);
//        timeClock.checkTime();
//        lcd.setCursor(0,1);
//        snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
//        lcd.print(timeString);
//      }else if(buttonStateMINUS==buttonDOWN){
//        timeClock.minusSeconds(1);
//        timeClock.checkTime();
//        lcd.setCursor(0,1);
//        snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
//        lcd.print(timeString);
//      }
//  }
//    if(millis() - lastSetupTime>=1000){
//      Serial.println(setupState);
//      lastSetupTime=millis();
//      timeClock.addSeconds(1);
//      timeClock.checkTime();
//      if(setupState==RUNNING){
//        lcd.setCursor(0,1);
//        snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", timeClock.getHour(),timeClock.getMinutes(),timeClock.getSeconds());
//        //timeClock.printTime();
//        lcd.print(timeString);
//      }
//    }
//  }
}
