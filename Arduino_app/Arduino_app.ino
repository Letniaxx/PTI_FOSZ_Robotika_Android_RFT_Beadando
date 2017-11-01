#include <Wire.h>                                                 //lcd-hez kell
#include <LiquidCrystal_I2C.h>                                    //lcd-hez kell
#include <SoftwareSerial.h>                                       //bt-hoz kell 

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);    //lcd-hez kell

//PIN-ek
const int rxPin = 0;
const int txPin = 1;
const int kintiVilagitas = 12;
const int bentiVilagitas = 11;
const int termosztatFutson = 10;
const int pir = 9;
const int hofokPin = 0;

//hőfokméréshez
const int numReadings = 10;         //hány mérést átlagoljon
int temp;                           //aktuális mérés
int readings[numReadings];          //mérések eredményei
int readIndex = 0;                  //a tömb indexe
int total = 0;                      //mérések összege
int average = 0;                    //mérések átlaga, legpontosabb!!!
int average_prev = 0;               //előző mérésátlag

//változók
int kivantHofok = 28;               //a kívánt hőfok a szobában
char btFogadottAdat = ' ';          //BT app által küldött karakter

SoftwareSerial bluetooth(rxPin, txPin);   //létrehoz egy bt példányt

void setup()
{  
  bluetooth.begin(9600);
  delay(200);
  bluetooth.print("Initializing...");
  lcd.begin(16,2);
  lcd.backlight();  
  
  //kimenetek
  pinMode(kintiVilagitas, OUTPUT);
  pinMode(bentiVilagitas, OUTPUT);
  pinMode(termosztatFutson, OUTPUT);
  
  //bemenetek
  pinMode(pir, INPUT);
  pinMode(hofokPin, INPUT);  

  //mérések átlagait beállítja 0-ra
  for (int thisReading = 0; thisReading < numReadings; thisReading++) 
    {
      readings[thisReading] = 0;
    }
  delay(1000);
}

void loop()
{       
  hofokMeres();                 //eredménye az: average
  lcdKijelzes();                //LCD kijelző frissítése
  bluetoothEsemenyek();
  kazanVezerles();

}

void hofokMeres()
{
  temp = ((5.0 * analogRead(hofokPin) * 100.0) / 1024) -2 ;
  total = total - readings[readIndex];
  readings[readIndex] = temp;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) 
    {
      readIndex = 0;
    }
  average = total / numReadings;
  delay(1);
}

void lcdKijelzes()
{
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp:");  
  lcd.setCursor(9,0);  
  lcd.print("Set:"); 
  lcd.setCursor(0,1);
  lcd.print("Fogadott:"); 
  
  lcd.setCursor(6,0);
  lcd.print(average);  
  lcd.setCursor(14,0);
  lcd.print(kivantHofok);
  lcd.setCursor(11,1);
  lcd.print(btFogadottAdat); 
}

void bluetoothEsemenyek()
{
  //bejövő parancsok figyelése
  if (bluetooth.available())
    {
      btFogadottAdat = bluetooth.read();
              
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

  //kimenő parancsok
  if (average_prev != average)
    {  
      Serial.println(average);    
      bluetooth.println(average);
      average_prev = average;
    }
  
}

void kazanVezerles()
{  
  if (kivantHofok > average)
  {
    digitalWrite(termosztatFutson, HIGH);
  }
  else if (kivantHofok < average)
  {
    digitalWrite(termosztatFutson, LOW);   
  }
}
