//3
#include <DS3232RTC.h>
#include <TimeLib.h>
#include <Streaming.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define CLK 2
#define DT 3
#define SW 4
const unsigned long KEY_HOLD=100;
unsigned long buttonPressTime;
bool buttonDown=false;
enum clockState { UNDEFINED, RUNNING, SETSEC, SETMIN, SETHOUR } setupState;
unsigned long int lastSetupTime=0;
int counter=0;
int currentStateCLK;
int lastStateCLK;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void setup(){
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(115200);
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  lastStateCLK=digitalRead(CLK);
  lcd.setCursor(0,0);
  lcd.print("WELCOME");
  setupState=RUNNING;
  setSyncProvider(RTC.get);
  Serial << F("RTC Sync");
  if (timeStatus() != timeSet) Serial << F(" FAIL!");
}
void loop()
{
    static time_t tLast;
    time_t t;
    tmElements_t tm;

    // check for input to set the RTC, minimum length is 12, i.e. yy,m,d,h,m,s
    if (Serial.available() >= 12) {
        // note that the tmElements_t Year member is an offset from 1970,
        // but the RTC wants the last two digits of the calendar year.
        // use the convenience macros from the Time Library to do the conversions.
        int y = Serial.parseInt();
        if (y >= 100 && y < 1000)
            Serial << F("Error: Year must be two digits or four digits!") << endl;
        else {
            if (y >= 1000)
                tm.Year = CalendarYrToTm(y);
            else    // (y < 100)
                tm.Year = y2kYearToTm(y);
            tm.Month = Serial.parseInt();
            tm.Day = Serial.parseInt();
            tm.Hour = Serial.parseInt();
            tm.Minute = Serial.parseInt();
            tm.Second = Serial.parseInt();
            t = makeTime(tm);
            RTC.set(t);        // use the time_t value to ensure correct weekday is set
            setTime(t);
            printDateTime(t);
            // dump any extraneous input
            while (Serial.available() > 0) Serial.read();
        }
    }

    t = now();
    if (t != tLast) {
      if(setupState==RUNNING){
        tLast = t;
        printDateTime(t);
      }
        if (second(t) == 0) {
            float c = RTC.temperature() / 4.;
            float f = c * 9. / 5. + 32.;
            if(setupState==RUNNING){
              printDateTimeTemperature(t,c,f);
            }
        }
    }
    if(digitalRead(SW)==LOW){
      delay(100);
      if(digitalRead(SW)==LOW){
      if(millis()-lastSetupTime>50){
        Serial.println("Button pressed!");
        lastSetupTime=millis();
        Serial.println(setupState);
        counter=0;
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
     }
   }
   currentStateCLK = digitalRead(CLK);
   if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    lastSetupTime=millis();
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      Serial.println(counter);
    } else {
      counter ++;
      Serial.println(counter);
    }
    if(setupState==SETHOUR){
        Serial.println(setupState);
        tm.Hour+=counter;
        t = makeTime(tm);
        RTC.set(t);        
        setTime(t);
        printDateTime(t);
      }else if(setupState==SETMIN){
        Serial.println(setupState);
        tm.Minute+=counter;
        t = makeTime(tm);
        RTC.set(t);        
        setTime(t);
        printDateTime(t);
      }else if(setupState==SETSEC){
        Serial.println(setupState);
        tm.Second+=counter;
        t = makeTime(tm);
        RTC.set(t);        
        setTime(t);
        printDateTime(t);
      }
   }
   lastStateCLK=currentStateCLK;
}
// print date and time to Serial
void printDateTime(time_t t){
    printDate(t);
    printTime(t);
}
// print time to Serial
void printTime(time_t t){
    lcd.setCursor(0,1);
    printI00(hour(t), ':');
    lcd.setCursor(3,1);
    printI00(minute(t), ':');
    lcd.setCursor(6,1);
    printI00(second(t), ' ');
}
// print date to Serial
void printDate(time_t t){
    lcd.setCursor(0,0);
    printI00(day(t), 0);
    lcd.print(monthShortStr(month(t)));
    lcd.print(year(t));
    lcd.print(' ');
   // Serial << monthShortStr(month(t)) << _DEC(year(t));
}
// Print an integer in "00" format (with leading zero),
// followed by a delimiter character to Serial.
// Input value assumed to be between 0 and 99.
void printI00(int val, char delim){
    if (val < 10) lcd.print("0");
    lcd.print(val);
    if (delim > 0) lcd.print(delim);
    return;
}
void printDateTimeTemperature(time_t t,float c,float f){
    printDateTime(t);
    lcd.setCursor(10,1);
    lcd.print(c);
    lcd.print("C");
}
