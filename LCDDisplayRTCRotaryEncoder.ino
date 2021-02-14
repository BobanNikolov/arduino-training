#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include "PushButton.h"
DS3231 RTC;
LiquidCrystal_I2C lcd(0x27,16,2);
#define CLK 2
#define DT 3
#define SW 4
#define buttonUP 2
#define buttonDOWN 1
#define buttonNOCHANGE 3
boolean setupDone=false;
enum clockState { UNDEFINED=0, RUNNING, SETDAY, SETMON, SETYEAR, SETDOW, SETHOUR, SETMIN, SETSEC } setupState;
PushButton buttonSETUP(SW);
bool help=false;
bool century = false;
bool h12Flag=false;
bool pmFlag=false;
String DoW[7]={"MON","TUE","WED","THU","FRI","SAT","SUN"};
int counter=0;
int currentStateCLK;
int lastStateCLK;
byte buttonStateSETUP=buttonNOCHANGE;
void checkSwitchCondition(void){
  buttonStateSETUP=buttonSETUP.stateChanged();
  if(buttonStateSETUP==buttonUP){
    Serial.println("Button pressed!");
    Serial.println(setupState);
  if(setupState==RUNNING){
        Serial.println(setupState);
        setupState=SETDAY;
        lcd.setCursor(1,0);
        lcd.blink();
      }else if(setupState==SETDAY){
        Serial.println(setupState);
        setupState=SETMON;
        lcd.setCursor(1,0);
        lcd.noBlink();
        lcd.setCursor(4,0);
        lcd.blink();
      }else if(setupState==SETMON){
        Serial.println(setupState);
        setupState=SETYEAR;
        lcd.setCursor(5,0);
        lcd.noBlink();
        lcd.setCursor(9,0);
        lcd.blink();
      }else if(setupState==SETYEAR){
        Serial.println(setupState);
        setupState=SETDOW;
        lcd.setCursor(9,0);
        lcd.noBlink();
        lcd.setCursor(13,0);
        lcd.blink();
      }else if(setupState==SETDOW){
        Serial.println(setupState);
        setupState=SETHOUR;
        lcd.setCursor(13,0);
        lcd.noBlink();
        lcd.setCursor(1,1);
        lcd.blink();
      }else if(setupState==SETHOUR){
        Serial.println(setupState);
        lcd.setCursor(1,1);
        lcd.noBlink();
        lcd.setCursor(4,1);
        lcd.blink();
        setupState=SETMIN;
      }else if(setupState==SETMIN){
        Serial.println(setupState);
        lcd.setCursor(4,1);
        lcd.noBlink();
        lcd.setCursor(7,1);
        lcd.blink();
        setupState=SETSEC;
      }else if(setupState==SETSEC){
        Serial.println(setupState);
       lcd.setCursor(7,1);
        lcd.noBlink();
        setupState=RUNNING;
      }
  }
}
void printToLCD(){
  lcd.setCursor(0,0);
  lcd.print(RTC.getDate(),DEC);
  lcd.setCursor(2,0);
  lcd.print("/");
  lcd.setCursor(3,0);
  lcd.print(RTC.getMonth(century),DEC);
  lcd.setCursor(5,0);
  lcd.print("/");
  lcd.setCursor(6,0);
  lcd.print("20");
  lcd.setCursor(8,0);
  lcd.print(RTC.getYear(),DEC);
  lcd.setCursor(11,0);
  lcd.print(DoW[RTC.getDoW()-1]);
  if(RTC.getHour(h12Flag,pmFlag)<10){
    lcd.setCursor(1,1);
  }else{
    lcd.setCursor(0,1);
  }
  lcd.print(RTC.getHour(h12Flag,pmFlag),DEC);
  lcd.setCursor(2,1);
  lcd.print(":");
  if(RTC.getMinute()<10){
    lcd.setCursor(4,1);
  }else{
    lcd.setCursor(3,1);
  }
  lcd.print(RTC.getMinute(),DEC);
  lcd.setCursor(5,1);
  lcd.print(":");
  lcd.setCursor(6,1);
  lcd.print(RTC.getSecond(),DEC);
  lcd.setCursor(11,1);
  lcd.print(RTC.getTemperature(),1);
  lcd.setCursor(15,1);
  lcd.print((char)223);
}
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  Wire.begin();
  Serial.begin(9600);
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(CLK);
  setupState=RUNNING;
  lcd.setCursor(0,0);
  lcd.print("SET ME UP");
}
byte getCounter(byte num){
  currentStateCLK = digitalRead(CLK);
   if (currentStateCLK != lastStateCLK&&currentStateCLK==1){
    if (digitalRead(DT) != currentStateCLK) {
      num --;
      Serial.println(num);
    } else {
      num ++;
      Serial.println(num);
    }
   }
   lastStateCLK=currentStateCLK;
   return num;
}
void loop() {
  // put your main code here, to run repeatedly:
  if(setupDone){
    
    printToLCD();
  }else{
    checkSwitchCondition();
   if(setupState==SETDAY){
    lcd.clear();
    byte currDay=RTC.getDate();
    byte setupDate=getCounter(currDay);
    RTC.setDate(setupDate);
    lcd.setCursor(0,0);
    lcd.print(RTC.getDate(),DEC);
    lcd.setCursor(2,0);
    lcd.print("/");
   }else if(setupState==SETMON){
    byte currMonth=RTC.getMonth(century);
    byte setupMonth=getCounter(currMonth);
    RTC.setMonth(setupMonth);
    lcd.setCursor(3,0);
    lcd.print(RTC.getMonth(century),DEC);
    lcd.setCursor(5,0);
    lcd.print("/");
   }else if(setupState==SETYEAR){
    byte currYear=RTC.getYear();
    byte setupYear=getCounter(currYear);
    RTC.setYear(setupYear);
    lcd.setCursor(6,0);
    lcd.print("20");
    lcd.setCursor(8,0);
    lcd.print(RTC.getYear(),DEC);
    lcd.setCursor(11,0);
    lcd.print(DoW[RTC.getDoW()-1]);
   }else if(setupState==SETDOW){
    byte currDoW=RTC.getDoW();
    byte setupDoW=getCounter(currDoW);
    RTC.setDoW(setupDoW);
    lcd.setCursor(6,0);
    lcd.print("20");
    lcd.setCursor(8,0);
    lcd.print(RTC.getYear(),DEC);
    lcd.setCursor(11,0);
    lcd.print(DoW[RTC.getDoW()-1]);
   }else if(setupState==SETHOUR){
    byte currHour=RTC.getHour(h12Flag,pmFlag);
    byte setupHour=getCounter(currHour);
    RTC.setHour(setupHour);
    lcd.setCursor(0,1);
    lcd.print(RTC.getHour(h12Flag,pmFlag),DEC);
    lcd.setCursor(2,1);
    lcd.print(":");
   }else if(setupState==SETMIN){
    byte currMinutes=RTC.getMinute();
    byte setupMinutes=getCounter(currMinutes);
    RTC.setMinute(setupMinutes);
    lcd.setCursor(3,1);
    lcd.print(RTC.getMinute(),DEC);
    lcd.setCursor(5,1);
    lcd.print(":");
   }else if(setupState==SETSEC){
    byte currSecond=RTC.getSecond();
    byte setupSecond=getCounter(currSecond);
    RTC.setSecond(setupSecond);
    lcd.setCursor(6,1);
    lcd.print(RTC.getSecond(),DEC);
    help=true;
   }else if(help){
    setupDone=true;
   }
  }
}
