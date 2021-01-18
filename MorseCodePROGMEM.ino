#define buz 8      // output audio on pin 8
#define note 1400      // music note/pitch
#define element 50
#define dot element
#define dash (element * 3)
#define interchardelay (element * 3)
#define interworddelay (element * 7)
const byte led = 12;

const byte messageString[] PROGMEM = "  Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.  ";

const byte MorseCodeArray[] PROGMEM = {
  0, 0, 0x52, 0, 0, 0, 0, 0x5E, 0x6D, 0x6D, 0, 0, 0x73, 0x61, 0x55, 0x32,             // Special chars
  0x3F, 0x2F, 0x27, 0x23, 0x21, 0x20, 0x30, 0x38, 0x3C, 0x3E, 0x78, 0, 0, 0, 0, 0x4C, // 0-9, :
  0, 5, 0x18, 0x1A, 0xC, 2, 0x12, 0xE, 0x10, 4, 0x17, 0xD, 0x14, 7, 6, 0xF,           // A-O
  0x16, 0x1D, 0xA, 8, 3, 9, 0x11, 0xB, 0x19, 0x1B, 0x1C, 0, 0, 0, 0, 0,               // P-Z
  0, 5, 0x18, 0x1A, 0xC, 2, 0x12, 0xE, 0x10, 4, 0x17, 0xD, 0x14, 7, 6, 0xF,           // a-o
  0x16, 0x1D, 0xA, 8, 3, 9, 0x11, 0xB, 0x19, 0x1B, 0x1C, 0, 0, 0, 0, 0                // p-z
};

void setup() {
  Serial.begin(9600);
    pinMode(led, OUTPUT);
    pinMode(buz, OUTPUT);
    Serial.println("I am ready...");
}

void BlinkDot()
{
  Serial.print(".");
    digitalWrite(led, HIGH);
    tone(buz,note);
    delay(dot);
    digitalWrite(led, LOW);
   noTone(buz);
    delay(interchardelay);
}

void BlinkDash()
{
  Serial.print("-");
    digitalWrite(led, HIGH);
    tone(buz,note);
    delay(dash);
    digitalWrite(led, LOW);
    noTone(buz);
    delay(interchardelay);
}

void EndOfLetter()
{
  delay(interworddelay*2);
}

void EndOfWord()
{
  delay(interworddelay);
}

void BlinkLetterCode(byte LetterCode)
{
  if (LetterCode > 1)
  {
    BlinkLetterCode(LetterCode >> 1);
    if (LetterCode & 1)
      BlinkDash();
    else
      BlinkDot();
  }
  else
    EndOfLetter();
}

void loop() {
  int i;
  char ch;

  for (i = 0;; ++i)
  {
    ch = pgm_read_byte(messageString + i);
    if (ch == 0)
      break;
    if (ch == ' ')
      EndOfWord();
    else if (ch > ' ' && ch <= 0x7F)
    {
      ch = pgm_read_byte(MorseCodeArray + ch - 0x20);
      BlinkLetterCode(ch);
    }
  }
}
