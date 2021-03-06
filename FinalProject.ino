//including needing libraries
#include <SdFat.h>
#include <SimpleDHT.h>
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include "PushButton.h"
//defining needing variables (used so I don't forget which pin is for what)
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
//initializing objects of given classes
DS3231 RTC;
RTClib rtc;
LiquidCrystal_I2C lcd(0x27,16,2);
SimpleDHT11 dht11(DHT_PIN);
SdFat sd;
File myFile;
PushButton buttonSETUP(SW);
char fileName[20]="\0"; //variable for keeping the name of the file
long duration; // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement
boolean setupDone=false; //flag for noticing when the setup is done
enum clockState { UNDEFINED=0, RUNNING, SETDAY, SETMON, SETYEAR, SETDOW, SETHOUR, SETMIN, SETSEC } setupState;//enum for state of setup button
byte temperature = 0; //measuring temperature
byte humidity = 0; //measuring humidity
int finalHumidity; //setting up humidity for printing
int finalTemperature; //setting up temperature for printing
int err = SimpleDHTErrSuccess; //checking if DHT sensor has any problems
bool help=false; //flag for checking when to print
bool century = false; //flag for .getMonth()
bool h12Flag=false; //flag for .getHour()
bool pmFlag=false; //flag for .getHour()
String DoW[7]={"MON","TUE","WED","THU","FRI","SAT","SUN"}; //string array for days of week
char timeString[50]; //char array for printing the current time
int counter=0; //counter for adding or subtracting while in setupmode
int timer;
int currentStateCLK;
int lastStateCLK;
byte buttonStateSETUP=buttonNOCHANGE;
//function for getting the current date and time and setting it up as the date and time on file that goes to SdCard
void dateTime(uint16_t* date, uint16_t* time, uint8_t* ms10) {
  DateTime now = rtc.now();

  // Return date using FS_DATE macro to format fields.
  *date = FS_DATE(now.year(), now.month(), now.day());

  // Return time using FS_TIME macro to format fields.
  *time = FS_TIME(now.hour(), now.minute(), now.second());

  // Return low time bits in units of 10 ms, 0 <= ms10 <= 199.
  *ms10 = now.second() & 1 ? 100 : 0;
}
//function for checking switch condition of setupState and settuping the state that we are in
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
//function to printing to LCD while checking is it correct time and date so it doesn't overlap
void printToLCD(){
  byte timeToCheck;
  if((int)RTC.getDate()<10){
    lcd.setCursor(0,0);
    lcd.print("0");
    lcd.setCursor(1,0);
  }else{
    lcd.setCursor(0,0);
  }
  lcd.print(RTC.getDate(),DEC);
  lcd.setCursor(2,0);
  lcd.print("/");
  if((int)RTC.getMonth(century)<10){
    lcd.setCursor(3,0);
    lcd.print("0");
    lcd.setCursor(4,0);
  }else{
    lcd.setCursor(3,0);
  }
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
//function to correct date
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
//function for adding or subtracting
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
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  Wire.begin();
  Serial.begin(9600);
  pinMode(DHT_PIN, INPUT_PULLUP);// Sets DHT sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(CLK,INPUT); //Sets rotary encoder
  pinMode(DT,INPUT);//Sets rotary encoder
  pinMode(SW, INPUT_PULLUP);//Sets rotary encoder
  sd.begin(10);//Sets SdCard
  lastStateCLK = digitalRead(CLK); //Sets rotary encoder
  setupState=RUNNING; //first state
  lcd.setCursor(0,0);
  lcd.print("SET ME UP");
}
void loop() {
  // put your main code here, to run repeatedly:
  if(setupDone){
    timer=(int)RTC.getSecond();
    if(timer==0||timer==20||timer==40){
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
      snprintf(timeString, sizeof(timeString), "bo%d%02d%02d.TXT", (int)RTC.getYear(), (int)RTC.getMonth(century), (int)RTC.getDate());
      Serial.println(timeString);
      strcpy(fileName,timeString);
      //strcat(fileName,".CSV");
      Serial.println(fileName);
      FsDateTime::setCallback(dateTime);
      myFile = sd.open(fileName, FILE_WRITE);
      if(myFile){
        Serial.println("SD CARD OKAY");
        snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d  %02d/%02d/%d  ", RTC.getHour(h12Flag,pmFlag),RTC.getMinute(),RTC.getSecond(),(int)RTC.getDate(), (int)RTC.getMonth(century), RTC.getYear());
        myFile.print(timeString);
        myFile.print(" Distance: "); myFile.print(distance); myFile.print(" cm");
        myFile.print(" Temperature: "); myFile.print(finalTemperature); myFile.print("°");
        myFile.print(" Humidity: "); myFile.println(finalHumidity);
        myFile.close();
      }else{
        Serial.println("SD CARD NOT OKAY");
      }
      strcpy(fileName,"");
      delay(1000);
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
    currMonth=RTC.getMonth(century);
    if((int)currMonth<10){
      lcd.setCursor(3,0);
      lcd.print("0");
      lcd.setCursor(4,0);
    }else{
      lcd.setCursor(3,0);
    }
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
