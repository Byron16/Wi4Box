#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#define accel_module (0x53)
byte values[6] ;
char output[512];


int ln=0;
int lf=0;
int  lt=0;

int tt=0;
int tf=0;
int tc=0;

int  vx1=0;
int  vx2=0;
int  vy1=0;
int  vy2=0;


char tyh[10];

LiquidCrystal lcd(7, 8, 9, 10, 11 , 12);

void setup() {
  dht.begin();
  Wire.begin();
  Serial.begin(9600);

  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(16);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.write("Iniciando...");
  delay(3000);
  lcd.setCursor(0,1);
  lcd.write("Bienvenido ");
  delay(3000);

  lcd.clear();
}

void loop() {  

  int t=dht.readTemperature(); //float
  sprintf(tyh,"T=%dC H=%d%%",t);
  Serial.println(tyh);
  lcd.setCursor(0,0);
  lcd.write(tyh);
  
  
  //
  Nivel=analogRead(A0);
  Nivel=map(NivelHumo,0,1023,0,100);
  lcd.setCursor(12,0);
  lcd.write("I=N");
  
  //
  Valor=analogRead(SensorLluvia);
  Valor=map(ValorLluvia,0,1023,0,100);
  lcd.setCursor(0,1);
  
  EstadoSonido=digitalRead(Sensor);
    lcd.setCursor(5,1);

  int xyzregister = 0x32;
  int x, y, z;
  
  Wire.beginTransmission(accel_module);
  Wire.write(xyzregister);
  Wire.endTransmission();
  
  Wire.beginTransmission(accel_module);
  Wire.requestFrom(accel_module, 6);
  
  int i = 0;
  while(Wire.available())
  {
    values[i] = Wire.read();
    i++;
   }
  Wire.endTransmission();

   

 // Serial.println(vx1);
 // Serial.println(vx2);
/*   Serial.println(vy1);
        Serial.println(vy2);*/
 // sprintf(output, "%d %d %d %d", vx1,vx2, vy1,vy2); 
 // Serial.println(output); 
  

  
  x = (((int)values[1]) << 8) | values[0]; 
  y = (((int)values[3])<< 8) | values[2]; 
  z = (((int)values[5]) << 8) | values[4]; 

/*  vx1=x-4;
  vx2=x+4;
  vy1=y-4;
  vy2=y+4;
  Serial.println(vx1);
  Serial.println(vx2);*/

  if ((x<=10)||(x>=19)||(y<=15)||(y>=25)) //x=37 y=26
  {
    Serial.println("S");
  }
  
  
  //sprintf(output, "x:%d y:%d z:%d", x, y, z); 
  //Serial.println(output); 

  //Serial.write(10); 
  //Serial.println("\n");
  lcd.setCursor(10,1);
  lcd.write("S=N");
}
