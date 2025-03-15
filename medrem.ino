#include <EEPROM.h>
#include <DS3232RTC.h>
#include <LiquidCrystal.h> // includes the LiquidCrystal Library
#include <Keypad.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte rowPins[ROWS] = {7,8,9,10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,1,0}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int buzzer = 13;
String storedHour;
String storedMinute;

void storeRepeatHour(String);
void storeRepeatMinute(String);

void setup() {

  //Serial.begin(9600);
  setSyncProvider(RTC.get); 
  lcd.begin(16, 2);
  lcd.clear();

  //storedHour = getHour();
  //storedMinute = getMinute();

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

}

void loop() {
  char key;
  int gapTime;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Medicin Reminder");
  lcd.setCursor(0, 1);
  lcd.print("Now:");
  lcd.setCursor(5, 1);
  digitalClockDisplay();

  delay(2000);
  
  while(1){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Repeation / day?");
    lcd.setCursor(0, 1);
    while(1){
      key = keypad.getKey();
      if (key != '\0') break;
    }
    lcd.print(key);
    switch(key){
      case '2':   gapTime = 12;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("2 times a day?");
                  lcd.setCursor(0, 1);
                  lcd.print("*Cancel #Confirm");
                  while(1){
                    key = keypad.getKey();
                    if (key == '#') break;
                    if (key == '*') { key='\0'; break; }
                  }
                  break;
                  
      case '3':   gapTime = 8;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("3 times a day?");
                  lcd.setCursor(0, 1);
                  lcd.print("*Cancel #Confirm");
                  while(1){
                    key = keypad.getKey();
                    if (key == '#') break;
                    if (key == '*') { key='\0'; break; }
                  }
                  break;
                  
      case '4':   gapTime = 6;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("4 times a day?");
                  lcd.setCursor(0, 1);
                  lcd.print("*Cancel #Confirm");
                  while(1){
                    key = keypad.getKey();
                    if (key == '#') break;
                    if (key == '*') { key='\0'; break; }
                  }
                  break;
                 
    }
    if (key == '#') { key='\0'; break; }
    if (key == '*') key='\0';

  }
  int d=0;
  String beginHour;
  
  while(key != '#' || d<=2){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Set Begin Hour:");
    lcd.setCursor(4, 1);
    lcd.print(" #Ok *Cancel");
    lcd.setCursor(0, 1);
    //String beginHour = getInputFromKeyPad(2);
  
    beginHour += key;
    key = keypad.getKey();
    if(key) d++;
    lcd.print(key);
    if (key == '*') { key='\0'; d=0; } 
  }
  String mediMinute = ""+minute();
  String mediHour[4];
  
  for (int i=0; i<24; i=i+gapTime)
    mediHour[i/gapTime] = (beginHour.toInt()+i);

  delay(3000);

  int clk=0;

  while (1) {
    lcd.clear();
    
    //storeRepeatHour(mediHour[clk/gapTime]);
    //storeRepeatMinute(mediMinute);
    storedHour = mediHour[clk/gapTime];
    storedMinute = mediMinute;

    String realHour = ""+hour();
    String realMinute = ""+minute();

    if (realHour.equals(storedHour) && realMinute.equals(storedMinute)) {
      soundAlarm();
    }

    lcd.setCursor(0, 0);
    lcd.print("Medication Time:");
    lcd.setCursor(0, 1);
    String MediTime = mediHour[clk/gapTime] + ":" + mediMinute + ":00";
    lcd.print(MediTime);
    delay(1000);

    clk = clk + gapTime;
    if (clk == 24) clk = 0;

  }
}

void digitalClockDisplay()
{
    // digital clock display of the time
    lcd.print(hour());
    printDigits(minute());
    printDigits(second());
}

void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    lcd.print(':');
    if(digits < 10)
        lcd.print('0');
    lcd.print(digits);
}

/*access to the string-input char by char*/
String getInputFromKeyPad(int n) {

  String num = "";
  String number;
  String confirmNumber;
  int i = 0;

  while (1) {
    
    char key = keypad.getKey();
    
    if (key) {

      if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9') {
        i++;
        if (i <= n) {
          num += key;
        }

        //Serial.println(num);
        
        lcd.setCursor(14, 0);
        lcd.clear();
        lcd.print(num);
        
        lcd.setCursor(0, 1);
        lcd.print("*Cancel #Confirm");

      }

      if (key == '#' && i == n) {
        break;
      }

      if (key == '*') {
        number = "";
        confirmNumber = "";
        num = "";
        return "";
      }

      if (key == '#' && i < n) {
        lcd.clear();
        lcd.print("Invalid Number!");
        delay(2000);
        number = "";
        confirmNumber = "";
        num = "";
        return "";

      }

      if (i > n) {
        lcd.clear();
        lcd.print("Invalid Option!");
        delay(2000);
        number = "";
        confirmNumber = "";
        num = "";
        return "";
      }

      else {
        lcd.clear();
        lcd.print("Invalid Option!");
        delay(2000);
        number = "";
        confirmNumber = "";
        num = "";
        return "";

      }

    }

  }
  return num;
}

/* makes alarming beep sounds. */
void soundAlarm() {
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(500); 
}

/*..........................................................................*/

void storeRepeatHour(String n) {
  for (int k = 0; k < 2; k++) {
    EEPROM.write(k, String(n[k]).toInt());
  }

}

/*..........................................................................*/

void storeRepeatMinute(String n) {
  for (int k = 0; k < 2; k++) {
    EEPROM.write(k+2, String(n[k]).toInt());
  }

}

/*..........................................................................*/

String getHour() {
  String newHour = "";
  for (int i = 0; i < 2; i++) {
    newHour += EEPROM.read(i);
    Serial.println(EEPROM.read(i));
  }
  return newHour;
}

/*---------------------------------------------------------------------------*/

String getMinute() {
  String newMinute = "";
  for (int i = 2; i <4; i++) {
    newMinute += EEPROM.read(i);
    Serial.println(EEPROM.read(i));
  }
  return newMinute;
}
