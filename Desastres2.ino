#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#define accel_module (0x53)
byte values[6] ;
char output[512];

int SensorHumo = A0;
int NivelHumo = 0;
int SensorLluvia = A1;
int ValorLluvia = 0;
int SensorSonido = 4;
int EstadoSonido;
int i=0;

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
  pinMode(SensorLluvia,INPUT);
  pinMode(ValorLluvia,OUTPUT);
  pinMode(SensorSonido,INPUT);
  
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
  TemyHum();
  Humo();
  Lluvia();
  Sonido();
  Movimiento();
  delay(1000);
}

void TemyHum(){
  int t=dht.readTemperature(); //float
 
  //sprintf(tyh,"T=%dC H=%d%%",t);
  //Serial.println(tyh);
 if ((t<=12)&&(tf==0))
 {
  Serial.println("R");
  tf=1;
  tc=0;
  tt=0;
 }

   if((t>13)&&(t<=25)&&(tc==0)){
     Serial.println("T");
     tf=0;
     tc=1;
     tt=0;
   }
   if((t>25)&&(tt==0))
   {
   Serial.println("C");
   tt=1;
   tf=0;
   tc=0;
   }
   //Serial.println("C");
  lcd.setCursor(0,0);
  lcd.write(tyh);

}

void Humo(){
  NivelHumo=analogRead(A0);
  NivelHumo=map(NivelHumo,0,1023,0,100);
  lcd.setCursor(12,0);
  if (NivelHumo>=50)
  {
    //Serial.println("Posible Incendio: "+String(NivelHumo)+"%");
    Serial.println("I");
    lcd.write("I=S");
    //digitalwrite(13,HIGH);
  }
  /*else
  {*/
    //digitalWrite(13,LOW)
    //Serial.println("Ningun Incendio: "+String(NivelHumo)+"%");
    lcd.write("I=N");
  //}

  
  
}

void Lluvia(){
  ValorLluvia=analogRead(SensorLluvia);
  ValorLluvia=map(ValorLluvia,0,1023,0,100);
  lcd.setCursor(0,1);
  if((ValorLluvia>80)&&(ln==0))
  {
    //Serial.println("Seco: "+String(ValorLluvia));
      Serial.println("N");
      lcd.write("LL=N");
      ln=1;
      lf=0;
      lt=0;
  }
  else
  if (ValorLluvia>50 && ValorLluvia<=80)
  { 
    //Serial.println("Lluvia Fina: "+String(ValorLluvia));
    if (lf==0){
      Serial.println("L");
      lcd.write("LL=S");
      lf=1;
      ln=0;
      lt=0;
    }
  }
  else
  if (ValorLluvia<=50){
    //Serial.println("LLuvia Fuerte: "+String(ValorLluvia));
    if(lt==0){
      Serial.println("F");
      lcd.write("LL=F");
      lt=1;
      lf=0;
      ln=0;
    }
  }
}

void Sonido(){
  EstadoSonido=digitalRead(SensorSonido);
    lcd.setCursor(5,1);
  if(EstadoSonido==LOW)
  {
    i++;
    if(i==40)
    {
      //Serial.println(" Posible tala de Arboles: "+String(i));
      Serial.println("A");
      lcd.write("TA=S");
      i=0;
    }
  }
/*  else
  {
      Serial.println("Ninguna Tala de Arboles: "+String(i));
      lcd.write("TA=N");
  }*/
  //delay(2);
}

void Movimiento()
{
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

/*R=Frio
T=templado
C=calido
I=incendio
N=ninguna lluvia
L=Lluvia fina
F=lluvia fuerte
A=tala de arboles
S=sismo*/
}
