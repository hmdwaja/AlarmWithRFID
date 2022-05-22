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

int i=1;
int k=5;
int Capteur= 6;

String password="1234";
String tempPassword;

boolean screenOffMsg = false;
boolean activated = false;
boolean isActivated;
boolean activateAlarm = false;
boolean alarmActivated = false;
boolean enteredPassword;
boolean passChangeMode = false;
boolean passChanged = false;

const byte ROWS = 4;
const byte COLS = 4;
char keypressed;
char keyMap[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {39, 38, 37, 36};
byte colPins[COLS] = {35, 34, 33, 32};

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS); 
void setup() 
{ 
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
void loop() 
{
 if (!screenOffMsg) 
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("A - Activate");
  lcd.setCursor(0,1);
  lcd.print("B - Change Pass");
 }
  keypressed = myKeypad.getKey();
  if (keypressed =='A' || digitalRead(RF_D0) == HIGH)
  {
   LedFlash ();
   screenOffMsg = false;
   activateAlarm = true;            
  }
  if (activateAlarm) 
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Alarm will be");
   lcd.setCursor(0,1);
   lcd.print("activated in");
   int countdown = 9;
   while (countdown != 0) {
   lcd.setCursor(13,1);
   lcd.print(countdown);
   countdown--;
   
   LedFlash ();
   
   delay(500);
  }
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Alarm Activated!");
   activateAlarm = false;
   alarmActivated = true;
  }
  if (alarmActivated == true)
  {
   if(digitalRead(Capteur)==HIGH)
   {
    digitalWrite(LED, HIGH);
    lcd.clear();
    enterPassword();
   }
  }
  else if (keypressed =='B') 
  {
   LedFlash ();
   
   tempPassword = "";
      
   CurrentPassword();
      
   passChangeMode = true;
   passChanged = true;   
   while(passChanged) 
   {
        
    keyPressed();
      
   }
   if (i > 5 || keypressed == '#') 
   {
    tempPassword = "";
    i=1;

    CurrentPassword();
   
   }
   if ( keypressed == '*') 
   {
    i=1;
    
    LedFlash ();
    
    if (password == tempPassword) 
    {
     tempPassword="";

     CurrentPassword();
          
     while(passChangeMode) 
     {
            
      keyPressed();
          
     }
    }
   }
   if (i > 5 || keypressed == '#') 
   {
    tempPassword = "";
    i=1;
    
    LedFlash ();
              
    CurrentPassword();
              
   }
   if ( keypressed == '*') 
   {
    i=1;
    
    LedFlash ();
    
    password = tempPassword;
    passChangeMode = false;
    passChanged = false;
    screenOffMsg = true;
   }            
  }
}
void keyPressed() 
{
 keypressed = myKeypad.getKey();  
 if (keypressed != NO_KEY)
 {
  if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ) 
  {
   keyPressed();
   tempPassword += keypressed;
   lcd.setCursor(i,1);
   lcd.print("*");
   i++;
   
   LedFlash ();
   
  }
 }
}
void LedFlash ()
{
 digitalWrite(LED, HIGH);
 delay(500);
 digitalWrite(LED, LOW);
}
void AlarmPass() 
{
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(" *** ALARM *** ");
 lcd.setCursor(0,1);
 lcd.print("Pass>");
}
void CurrentPassword()
{
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Current Password");
 lcd.setCursor(0,1);
 lcd.print(">"); 
}
void enterPassword() 
{ 
 tempPassword = "";
 activated = true;
  
 AlarmPass();
  
 while(activated) 
 {
  keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY)
  {
   if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
   keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
   keypressed == '8' || keypressed == '9' ) 
   {
    tempPassword += keypressed;
    lcd.setCursor(k,1);
    lcd.print("*");
    k++;
   }
  }
  if (k > 9 || keypressed == '#') 
  {
   tempPassword = "";
   k=5;
        
   AlarmPass();
        
  }
  if (keypressed =='A' || digitalRead(RF_D0) == HIGH)
  {
   if ( tempPassword == password ) 
   {
    activated = false;
    alarmActivated = false;
    digitalWrite(LED, LOW);
    screenOffMsg = true; 
   }
   else if (tempPassword != password) 
   {
    lcd.setCursor(0,1);
    lcd.print("Wrong! Try Again");
    delay(2000);
          
    AlarmPass();
    
   }    
  }
 }
}
