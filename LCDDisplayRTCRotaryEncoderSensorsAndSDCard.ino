//5
#include <SimpleDHT.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include "PushButton.h"
DS3231 RTC;
LiquidCrystal_I2C lcd(0x27,16,2);
#define echoPin 7
#define trigPin 6
#define buz 9
#define CLK 2
#define DT 3
#define SW 4
#define buttonUP 2
#define buttonDOWN 1
#define buttonNOCHANGE 3
#define DHTTYPE DHT11
#define DHT_PIN 8
SimpleDHT11 dht11(DHT_PIN);
File myFile;
long duration; // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement
boolean setupDone=false;
enum clockState { UNDEFINED=0, RUNNING, SETDAY, SETMON, SETYEAR, SETDOW, SETHOUR, SETMIN, SETSEC } setupState;
PushButton buttonSETUP(SW);
byte temperature = 0;
byte humidity = 0;
int finalHumidity;
int finalTemperature;
int err = SimpleDHTErrSuccess;
bool help=false;
bool century = false;
bool h12Flag=false;
bool pmFlag=false;
String DoW[7]={"MON","TUE","WED","THU","FRI","SAT","SUN"};
char timeString[50];
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
  byte timeToCheck;
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
  timeToCheck=RTC.getHour(h12Flag,pmFlag);
  if(timeToCheck<0){
    timeToCheck=24;
  }else if(timeToCheck>24){
    timeToCheck=0;
  }
  if(timeToCheck<10){
    lcd.setCursor(0,1);
    lcd.print("0");
    lcd.setCursor(1,1);
  }else{
    lcd.setCursor(0,1);
  }
  lcd.print(timeToCheck,DEC);
  lcd.setCursor(2,1);
  lcd.print(":");
  timeToCheck=RTC.getMinute();
  if(timeToCheck<0){
    timeToCheck=60;
  }else if(timeToCheck>60){
    timeToCheck=0;
  }
  if(timeToCheck<10){
    lcd.setCursor(3,1);
    lcd.print("0");
    lcd.setCursor(4,1);
  }else{
    lcd.setCursor(3,1);
  }
  lcd.print(timeToCheck,DEC);
  lcd.setCursor(5,1);
  lcd.print(":");
  timeToCheck=RTC.getSecond();
  if(timeToCheck<0){
    timeToCheck=60;
  }else if(timeToCheck>60){
    timeToCheck=0;
  }
  if(timeToCheck<10){
    lcd.setCursor(6,1);
    lcd.print("0");
    lcd.setCursor(7,1);
  }else{
    lcd.setCursor(6,1);
  }
  lcd.print(timeToCheck,DEC);
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
  pinMode(DHT_PIN, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  SD.begin(10);
  lastStateCLK = digitalRead(CLK);
  setupState=RUNNING;
  lcd.setCursor(0,0);
  lcd.print("SET ME UP");
}
byte correctDate(byte num){
  if (setupState==SETDAY){
    if(num<0){
      num=31;
    }else if(num>31){
      num=0;
    }
  }else if(setupState==SETMON){
    if(num<0){
      num=12;
    }else if(num>12){
      num=0;
    }
  }else if(setupState==SETHOUR){
    if(num<0){
      num=24;
    }else if(num>24){
      num=0;
    }
  }else if(setupState==SETMIN||setupState==SETSEC){
    if(num<0){
      num=60;
    }else if(num>60){
      num=0;
    }
  }
  return num;
}
byte getCounter(byte num){
  currentStateCLK = digitalRead(CLK);
   if (currentStateCLK != lastStateCLK&&currentStateCLK==1){
    if (digitalRead(DT) != currentStateCLK) {
      num--;
      num=correctDate(num);
      Serial.println(num);
    } else {
      num++;
      num=correctDate(num);
      Serial.println(num);
    }
   }
   lastStateCLK=currentStateCLK;
   return num;
}
void loop() {
  // put your main code here, to run repeatedly:
  if(setupDone){
    if((int)RTC.getSecond()==0){
      tone(buz,1400);
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin,LOW);
      duration = pulseIn(echoPin,HIGH);
      distance = duration * 0.034 / 2;
      Serial.print("Distance : "); Serial.println(distance);
      if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
        Serial.print("Read DHT11 failed, err="); Serial.println(err); 
      }else{
        finalHumidity=(int)humidity;
        finalTemperature=(int)temperature;
        Serial.print("Temperature : "); Serial.println(finalTemperature);
        Serial.print("Humidity : "); Serial.println(finalHumidity);
      }
      myFile=SD.open("boban.txt",FILE_WRITE);
      if(myFile){
        myFile.print("The time and date is now: ");
        snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d  %02d/%02d/%04d  ", RTC.getHour(h12Flag,pmFlag),RTC.getMinute(),RTC.getSecond(),(int)RTC.getDate(), (int)RTC.getMonth(century), RTC.getYear());
        myFile.print(timeString);
        myFile.println(DoW[RTC.getDoW()-1]);
        myFile.print("Distance: "); myFile.print(distance); myFile.println(" cm");
        myFile.print("Temperature: "); myFile.print(finalTemperature); myFile.println((char)248);
        myFile.print("Humidity: "); myFile.println(finalHumidity);
        myFile.println("-----------------------------------------");
        myFile.close();
      }
    }
    noTone(buz);
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
