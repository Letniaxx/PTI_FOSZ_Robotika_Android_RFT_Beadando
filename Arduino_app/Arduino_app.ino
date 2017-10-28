#include <Wire.h>                                                 //lcd-hez kell
#include <LiquidCrystal_I2C.h>                                    //lcd-hez kell
#include <SoftwareSerial.h>                                       //bt-hoz kell 
  
#define rxPin 0                                                   //bt-hoz kell
#define txPin 1                                                   //bt-hoz kell

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);    //lcd-hez kell

const int kintiVilagitas = 12;
const int bentiVilagitas = 11;
const int termosztatFutson = 10;
const int pir = 9;
const int potiPin = 1;
int poti;
int poti_elozo;

SoftwareSerial blueTooth(rxPin, txPin);                         //létrehoz egy bt példányt

void setup()
{
  Serial.begin(9600);
  blueTooth.begin(9600);
  pinMode(kintiVilagitas, OUTPUT);
  pinMode(bentiVilagitas, OUTPUT);
  pinMode(termosztatFutson, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(potiPin, INPUT);
  
  lcd.begin(16,2);
  lcd.backlight();  
}

void loop()
{   
  poti = analogRead(potiPin);
  poti = map(poti, 0, 1023, 0, 99);    
  
  if (Serial.available())
    {
      if (poti_elozo != poti)
      {
      Serial.println(poti);
      blueTooth.write(poti);
      poti_elozo = poti;
      }
      delay(20);

      char btFogadottAdat = blueTooth.read();
              
        switch(btFogadottAdat)
        {
          case 'a':   digitalWrite(bentiVilagitas, HIGH);   
                      break;
          
          case 'b':   digitalWrite(bentiVilagitas, LOW);         
                      break;
    
          case 'c':   digitalWrite(kintiVilagitas, HIGH);
                      break;
          
          case 'd':   digitalWrite(kintiVilagitas, LOW);
                      break;
          
          default:    break;
        }
    }
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hangero");  
  lcd.setCursor(0,1);
  lcd.print(poti); 
}
