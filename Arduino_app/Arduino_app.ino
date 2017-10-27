#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int kintiVilagitas = 12;
const int bentiVilagitas = 11;
const int termosztatFutson = 10;
const int pir = 9;
char btFogadottAdat = 0;


void setup()
{
  Serial.begin(9600);

  pinMode(kintiVilagitas, OUTPUT);
  pinMode(bentiVilagitas, OUTPUT);
  pinMode(termosztatFutson, OUTPUT);
  pinMode(pir, INPUT);
  
  lcd.begin(16,2);
  lcd.backlight();
}

void loop()
{    
  if(Serial.available() > 0)  // Send data only when you receive data:
  {
    btFogadottAdat = Serial.read();
    Serial.print(btFogadottAdat);
    Serial.print("\n");
    if (btFogadottAdat == '1')
    {
      digitalWrite(kintiVilagitas, HIGH);
      digitalWrite(bentiVilagitas, LOW);
      lcdSor1Kiir("Kinti");
    }
    else if (btFogadottAdat == '0')
    {
      digitalWrite(kintiVilagitas, LOW);
      digitalWrite(bentiVilagitas, HIGH);
      lcdSor1Kiir("Benti"); 
    }
  }          

  /*
  if (digitalRead(pir))
  {
    lcdSor2Kiir("Mozgas van");
  }
  else lcd.clear();
  */
}



void lcdSor1Kiir(char text1[5])
{
  lcd.clear();
  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print(text1);  
  //delay(10000);
}

void lcdSor2Kiir(char text2[10])
{
  //lcd.clear();  
  lcd.setCursor(0,1);
  lcd.print(text2);  
  //delay(10000);
}

