#include <LiquidCrystal_I2C.h>                                    //lcd-hez kell
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);    //lcd-hez kell

//PIN-ek
const int rxPin = 0;
const int txPin = 1;
const int kintiVilagitas = 12;
const int bentiVilagitas = 11;
const int termosztatFutson = 10;
const int pir = 9;
const int hofokPin = 0;
const int fenyPin = 1;
const int piezoPin = 8;

//hőfokméréshez
const int numReadings = 10;         //hány mérést átlagoljon
int temp;                           //aktuális mérés
int readings[numReadings];          //mérések eredményei
int readIndex = 0;                  //a tömb indexe
int total = 0;                      //mérések összege
int average = 0;                    //mérések átlaga, legpontosabb!!!

//változók
char btFogadottAdat = ' ';          //BT app által küldött karakter
int kivantHofok = 25;               //a kívánt hőfok a szobában
int mozgasVan = 0;                  //érzékel-e mozgást a pir
int riasztoBekapcsolva = 0;         //be van-e kapcsolva a roasztó
int ertekGyujtemeny[4] = {0,0,0,0}; //értékek gyűjteménye ami elküldésre kerül
int ertekGyujtemeny_[4] = {0,0,0,0}; 
int fenyero = 0;                    //a fotoellenállás aktuális értéke
int kintiVilagitasSzamlalo = 0;
int kintiVilagitasIdotartam = 15;
int kintiVilagitasFenyErzek = 300;


void setup()
{  
  Serial.begin(9600);
  delay(200);  
  lcd.begin(16,2);
  lcd.backlight();  
  
  //kimenetek
  pinMode(kintiVilagitas, OUTPUT);
  pinMode(bentiVilagitas, OUTPUT);
  pinMode(termosztatFutson, OUTPUT);
  
  //bemenetek
  pinMode(pir, INPUT);
  pinMode(hofokPin, INPUT);  
  pinMode(fenyPin, INPUT);

  //mérések átlagait beállítja 0-ra
  for (int thisReading = 0; thisReading < numReadings; thisReading++) 
    {
      readings[thisReading] = 0;
    }  
  delay(500);
}

void loop()
{       
  hofokMeres();                 //eredménye az: average
  lcdKijelzes();                //LCD kijelző frissítése
  bluetoothEsemenyek();         //bejövő parancsok figyelése
  kazanVezerles();              //hőfokok függvényében ki és bekapcsolja a kazánt
  mozgasErzekeles();   
  kintiVilagitasVezerles();         
  riasztoVezerles();
  ertekekOsszegyujtese();       //változók értékeit összegyűjti egy tömbbe
  ertekekKuldese();             //a tömb tartalmát serialon elküldi
  delay(200);                   //kis szünet 
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
  lcd.print("Alarm:"); 
  
  lcd.setCursor(6,0);
  lcd.print(average);  
  lcd.setCursor(14,0);
  lcd.print(kivantHofok);
  //RIASZTÓ
  lcd.setCursor(7,1);
  if (riasztoBekapcsolva == 0)
  {
    lcd.print("OFF");
  }
  else if (riasztoBekapcsolva == 1)
  {
    lcd.print("ON ");
  }
  //MOZGÁS
  lcd.setCursor(11,1);  
  if (mozgasVan == 0)
  {
    lcd.print("     ");
  }
  else if (mozgasVan == 1)
  {
    lcd.print("AKTIV");
  }
}

void bluetoothEsemenyek()
{
  //bejövő parancsok figyelése
  if (Serial.available() > 0)
    {
      btFogadottAdat = Serial.read();
              
        switch(btFogadottAdat)
        {
          case 'a':   digitalWrite(bentiVilagitas, HIGH);   
                      break;
          
          case 'b':   digitalWrite(bentiVilagitas, LOW);         
                      break;
    
          case 'c':   kintiVilagitasOn();
                      break;
          
          case 'd':   kintiVilagitasOff();
                      break;

          case 'e':   kivantHofokFel();
                      break;
          
          case 'f':   kivantHofokLe();
                      break;
    
          case 'g':   riasztoBekapcsolva = 1;
                      break;
          
          case 'h':   riasztoBekapcsolva = 0;
                      break;
          
          default:    break;
        }
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

void mozgasErzekeles()
{
  if (digitalRead(pir) == HIGH)
  {
    mozgasVan = 1;
    
    //ha mozgás és sötét van, akkor nullázza a számlálót, azaz indítja a kinti világítást
    if (fenyero < kintiVilagitasFenyErzek)
    {
      kintiVilagitasSzamlalo = 0;
    }    
  }
  else if (digitalRead(pir) == LOW)
  {
    mozgasVan = 0;
  }  
}

void kintiVilagitasVezerles()
{
  fenyero = analogRead(fenyPin);
    
  if (kintiVilagitasSzamlalo < kintiVilagitasIdotartam)
  {
    digitalWrite(kintiVilagitas, HIGH);
    kintiVilagitasSzamlalo++;
  }
  else
  {
    digitalWrite(kintiVilagitas, LOW);
  }
}

void kintiVilagitasOn()
{
  kintiVilagitasSzamlalo = 0;
}

void kintiVilagitasOff()
{
  kintiVilagitasSzamlalo = kintiVilagitasIdotartam;
}

void riasztoVezerles()
{
  if ( (riasztoBekapcsolva == 1) && (mozgasVan == 1) )
  {
    tone(piezoPin, 1000, 70);    
  }
  else
  {
    noTone(piezoPin);    
  }
}

void kivantHofokFel()
{
  if (kivantHofok < 35)
    kivantHofok++;
}

void kivantHofokLe()
{
  if (kivantHofok > 15)
    kivantHofok--;
}

void ertekekOsszegyujtese()
{  
  ertekGyujtemeny[0] = mozgasVan;
  ertekGyujtemeny[1] = riasztoBekapcsolva;
  ertekGyujtemeny[2] = kivantHofok;
  ertekGyujtemeny[3] = average;
}

void ertekekKuldese()
{  
   if ( (ertekGyujtemeny_[0] != ertekGyujtemeny[0]) ||
        (ertekGyujtemeny_[1] != ertekGyujtemeny[1]) ||
        (ertekGyujtemeny_[2] != ertekGyujtemeny[2]) ||
        (ertekGyujtemeny_[3] != ertekGyujtemeny[3]) )
    {
       Serial.print('#');                    //a csomag elejére: #  
       for(int k=0; k<4; k++)    
       {
         Serial.print(ertekGyujtemeny[k]);   //értékek
         Serial.print('+');                  //az értékek közé: +
       }  
       Serial.print('~');                    //a csomag végére: ~  
       Serial.println();                     //új sor
       

      (ertekGyujtemeny_[0] = ertekGyujtemeny[0]);
      (ertekGyujtemeny_[1] = ertekGyujtemeny[1]);
      (ertekGyujtemeny_[2] = ertekGyujtemeny[2]);
      (ertekGyujtemeny_[3] = ertekGyujtemeny[3]);
    }  
}
