#include <LiquidCrystal.h>
#include<Wire.h>
#include <Keypad.h>
#include <SPI.h>
#include <RFID.h>

#define RF_D0 2
#define RF_D1 3
#define RF_D2 4
#define RF_D3 5
#define LED 8
#define RST_PIN 9
#define SS_PIN 10

RFID rfid(SS_PIN, RST_PIN);
String rfidCard;
LiquidCrystal lcd(31, 30, 28, 26, 24, 22);

int Capteur= 6;
int screenOffMsg =0;
String password="1234";
String tempPassword;
boolean activated = false; // State of the alarm
boolean isActivated;
boolean activateAlarm = false;
boolean alarmActivated = false;
boolean enteredPassword; // State of the entered password to stop the alarm
boolean passChangeMode = false;
boolean passChanged = false;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keypressed;
//define the cymbols on the buttons of the keypads
char keyMap[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {39, 38, 37, 36}; //Row pinouts of the keypad
byte colPins[COLS] = {35, 34, 33, 32}; //Column pinouts of the keypad

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS); 
void setup() { 
  Serial.begin(9600);
  lcd.begin(20,4); 
  pinMode(LED, OUTPUT);
  pinMode(Capteur,INPUT);
  pinMode(RF_D0, INPUT_PULLUP);
  pinMode(RF_D1, INPUT_PULLUP);
  pinMode(RF_D2, INPUT_PULLUP);
  pinMode(RF_D3, INPUT_PULLUP);
  SPI.begin();
  rfid.init();
}
 void loop() {
if (activateAlarm) 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm will be");
    lcd.setCursor(0,1);
    lcd.print("activated in");
   
    int countdown = 9; // 9 seconds count down before activating the alarm
    while (countdown != 0) 
    {
      lcd.setCursor(13,1);
      lcd.print(countdown);
      countdown--;
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm Activated!");
    activateAlarm = false;
    alarmActivated = true;
  }

  if (alarmActivated){
        if(digitalRead(Capteur)==HIGH)
        {
          digitalWrite(LED, HIGH);
          lcd.clear();
          enterPassword();
        }
  }
  if (!alarmActivated) {
    if (screenOffMsg == 0 ){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A - Activate");
      lcd.setCursor(0,1);
      lcd.print("B - Change Pass");
      screenOffMsg = 1;
    }
    keypressed = myKeypad.getKey();
    
     if (keypressed =='A' || digitalRead(RF_D0) == HIGH){     
      digitalWrite(LED, HIGH);
      delay(1000);
      digitalWrite(LED, LOW);
      activateAlarm = true; 
     }
    else if (keypressed =='B') 
      {
      lcd.clear();
      int i=1;
      digitalWrite(LED, HIGH);
      delay(1000);
      digitalWrite(LED, LOW);
      tempPassword = "";
      lcd.setCursor(0,0);
      lcd.print("Current Password");
      lcd.setCursor(0,1);
      lcd.print(">");
      passChangeMode = true;
      passChanged = true;   
      
      while(passChanged) {      
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY)
      {
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) 
            {
               tempPassword += keypressed;
               lcd.setCursor(i,1);
               lcd.print("*");
               i++;
               digitalWrite(LED, HIGH);
               delay(1000);
               digitalWrite(LED, LOW);
            }
      }
      if (i > 5 || keypressed == '#') 
        {
          tempPassword = "";
          i=1;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Current Password");
          lcd.setCursor(0,1);
          lcd.print(">"); 
        }
      if ( keypressed == '*') 
      {
        i=1;
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
        if (password == tempPassword) 
        {
          tempPassword="";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set New Password");
          lcd.setCursor(0,1);
          lcd.print(">");
          while(passChangeMode) {
            keypressed = myKeypad.getKey();
            if (keypressed != NO_KEY){
              if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
                  keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                  keypressed == '8' || keypressed == '9' ) {
                tempPassword += keypressed;
                lcd.setCursor(i,1);
                lcd.print("*");
                i++;
                digitalWrite(LED, HIGH);
                delay(1000);
                digitalWrite(LED, LOW);
              }
            }
            if (i > 5 || keypressed == '#') {
              tempPassword = "";
              i=1;
              digitalWrite(LED, HIGH);
              delay(1000);
              digitalWrite(LED, LOW);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Set New Password");
              lcd.setCursor(0,1);
              lcd.print(">");
            }
            if ( keypressed == '*') 
              {
                i=1;
                digitalWrite(LED, HIGH);
                delay(1000);
                digitalWrite(LED, LOW);
                password = tempPassword;
                passChangeMode = false;
                passChanged = false;
                screenOffMsg = 0;
              }            
          }
        }
      }
    }
   }
 }
}

void enterPassword() {
  int k=5;
  tempPassword = "";
  activated = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("* ALARM ACTIVATED * ");
  lcd.setCursor(0,1);
  lcd.print("Pass>");
      while(activated) {
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
          tempPassword += keypressed;
          lcd.setCursor(k,1);
          lcd.print("*");
          k++;
        }
      }
      if (k > 9 || keypressed == '#') {
        tempPassword = "";
        k=5;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" *** ALARM *** ");
        lcd.setCursor(0,1);
        lcd.print("Pass>");
      }
      if ( keypressed == '*' || digitalRead(RF_D2) == HIGH) {
        if ( tempPassword == password ) {
          activated = false;
          alarmActivated = false;
          digitalWrite(LED, LOW);
          screenOffMsg = 0;    
        }
        else if (tempPassword != password) {
          lcd.setCursor(0,1);
          lcd.print("Wrong! Try Again");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" *** ALARM *** ");
          lcd.setCursor(0,1);
          lcd.print("Pass>");
        }
      }    
    }
}
