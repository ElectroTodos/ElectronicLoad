
/*    Firmware de ElectroTodos para Electronic Load
 *    En caso de dudas y/o consultas diígase a:  https://youtube.com/electronicatodos
 *    El proyecto se encuentra tanto en el canal antes mencionado.
 *    Decumentación adicional en: https://github.com/ElectroTodos/ElectronicLoad
 *    Se utilizan librerías externas bajo la tribución correspondiente.
 *    Relizado por Juan de ElectroTodos el 3/6/2016
 */
#include <Adafruit_MCP4725.h>          //https://github.com/adafruit/Adafruit_MCP4725
#include <ClickEncoder.h>              //https://github.com/0xPIT/encoder/tree/arduino
#include <TimerOne.h>
#include <PID_v1.h>                   //Librería necesaria para el control del cooler. Para descargarla: http://playground.arduino.cc/Code/PIDLibrary
#include <Wire.h>                     //Librería necesaria para la comunicación I2C
#include <LiquidCrystal_I2C.h>        //Librería para el LCD I2C. Para descargarla: https://github.com/marcoschwartz/LiquidCrystal_I2C
LiquidCrystal_I2C lcd(0x27,16,2);     //Iniciamos el LCD 16x2
float I, V, P, A = 0, B, R,Pset,Rset,T6;              //Variables varias
float R1=1.1628,Max=36.15,Pmax=75;    //R1 es el valor real de la resistencia usada en el circuito. Max es el valor para el cual el divisor de tensión de la carga tendrá 5v a su salida
int Arranque=90,mode=0,ShowTemp,mult,multmode,selector,Istart,Istop;                      //Valor de PWM al que el cooler puede arrancar
float Temperatura,Iset;
long double tiempo3,IsetD,TimeD;
#define PIN_INPUT 2                   //Entrada del sensor de temperatura
#define PIN_OUTPUT 3                  //Salida PWM para el cooler
#define AT 75                         //75
#define der 9
#define izq 10
#define pulsador 8
unsigned long tiempo,tiempo2,tiempo4,Time;                 //Tiempo de millis
bool Alerta=0,noDisp=0,RampaNoConf=1;
double Setpoint, Input, Output;
double Kp=2, Ki=5, Kd=1;              //Constantes del control PID de temperatura (no tocar a menos que se sepa lo que hacen)
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);
ClickEncoder *encoder;
int16_t last, value;
Adafruit_MCP4725 dac;
void timerIsr() {
  encoder->service();
}

                                      
void setup() {
  Serial.begin(9600);
 
  pinMode(der,INPUT);
  pinMode(izq,INPUT);
  pinMode(pulsador,INPUT);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  encoder = new ClickEncoder(der, izq, pulsador);
  mult=5;
  dac.begin(0x60);
  Setpoint = 19;                      //Temperatura a la que arranca el cooler (45)
  myPID.SetMode(AUTOMATIC);
  pinMode(5,INPUT);
  lcd.init();                         // Iniciamos todo lo necesario
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("ElectroTodos");
  lcd.setCursor(0,1);
  lcd.print("Electronic Load");
  delay(300);
  tone(4,1500,200);
  delay(300);
  tone(4,1500,200);
  delay(400);
  tone(4,1500,600);
  delay(2000);
  tiempo=millis();
  
 }

void loop()
{
  Cooler();
  Temp();
  Encoder();
  RunMode();
  Cuentas();                      //Se realizan las mediciones y cuentas necesarias
  Dac();
  if(noDisp==0) Display();
  serial();
}

void Cuentas()
{
  int W;                          //Promediamos las entradas obtenidas para mejorar un poco los resultados
   for (W = 1; W <= 500; W++)
  {
    I = analogRead(0);
    A = A + I;
    V = analogRead(1);
    B = V + B;
  }
  I = A / 500;
  V = B / 500;
  I = I * 1000 * 5 / (1024*R1);  //Calculamos la corriente en mA
  V = V*Max/1024;                //Calculamos la tensión en V
  P = V * I / 1000;              //Calculamos la potencia en W
  A = 0;
  B = 0;
}

void Tension()                //Se agregarn ceros y espacios necesarios para mostrar correctamente la tensión
{                             // y se la imprime por pantalla

  lcd.setCursor(0, 0);
  if (V >= 10)
  {
   lcd.print(V);
  }
  else
  {
    lcd.print("0");
    lcd.print(V);
  }
  lcd.setCursor(5, 0); 
  lcd.print("V");
 }

 
void Corriente()               //Se agregarn ceros y espacios necesarios para mostrar correctamente la corriente
{                              // y se la imprime por pantalla
    lcd.setCursor(8, 0);
    if (I >= 100)
  {
   lcd.print(I);
  }
  else if(I >=10)
  {
    lcd.print(" ");
    lcd.print(I);
  }
  else
  {
    lcd.print("  ");
    lcd.print(I);
  }
  lcd.setCursor(14, 0);
  lcd.print("mA");
 }

 void Potencia()               //Se agregarn ceros y espacios necesarios para mostrar correctamente la potencia
 {                             // y se la imprime por pantalla
  int j;
   lcd.setCursor(0, 1);
   if(P>Pmax)                   //Si la potencia supera la máxima que podemos disipar se activa una alerta
   {
    lcd.print("ALERTA");
    Alerta=1;
   }
   else
    {
    if(P>10)
    {
     lcd.print(P);
     lcd.print("W"); 
    }
    else
    {
      lcd.print("0");
      lcd.print(P);
      lcd.print("W"); 
    }
   }
    
 }

 void Resistencia()         //Se calcula la resistencia equivalente que tendría la carga
 {
   byte ohms[8] = {          //Se guarda el símbolo de Ohms
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B11011,
  B00000,
  };
  lcd.createChar(0, ohms);
  lcd.setCursor(8,1);
  if(I<0.01)
    {
      lcd.print("Infinita");            //Para I=0 la resistencia equivalente será infinita
   } 
   else
   {
    R=(V/I)*1000;
    if(R<10000)
    {
      if(R<1000)
      {lcd.print(" ");
      if(R<100){lcd.print(" ");}        //Se imprimen valores de R si es menor a 10k y se agregan espacios correspondientes
      if(R<10){lcd.print(" ");}}
      lcd.print(R);
      lcd.write(byte(0));
    }
    else
    {
      if(R<100000){lcd.print(" ");}
      lcd.print(R/1000);                //Se imprimen valores de R si es mayor a 10k y se agregan espacios correspondientes
      lcd.setCursor(14,1);
      lcd.print("k");
      lcd.write(byte(0));
    }
   }
 }

 
void Cooler()
{
  Input = (5*analogRead(PIN_INPUT)*100)/1024;
  myPID.Compute();
  if(Output<Arranque){Output=0;}
  analogWrite(PIN_OUTPUT, Output);
}

void Temp()
{
  Temperatura=(5*analogRead(PIN_INPUT)*100)/1024;
  if(Temperatura>AT)
  {
    Alerta=1;
    digitalWrite(7,LOW);
  }
  else
  {
    digitalWrite(7,HIGH);
  }
}

void MostrarTemperatura()
{
  byte grados[8] =
 {
    0b00001100,     
    0b00010010,
    0b00010010,
    0b00001100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
 };
  lcd.createChar(1, grados);
  lcd.setCursor(9,1);
  lcd.print(Temperatura);
  lcd.write(byte(1));
  lcd.print("C");
}

void serial()
{
  Serial.print(V);
  Serial.print(" V     ");
  Serial.print(I);
  Serial.print(" mA     ");
  Serial.print(R);
  Serial.print(" Ohms     ");
  Serial.print(P);
  Serial.print(" W     ");
  Serial.print(Temperatura);
  Serial.print(" C     ");
  Serial.print(Rset);
  Serial.print(" Ohms   ");
  Serial.print(Iset);
  Serial.print(" mA   ");
  Serial.print(millis());
  Serial.print(" mS\n");
}

void IsetShow()               //Se agregarn ceros y espacios necesarios para mostrar correctamente la corriente
{                              // y se la imprime por pantalla
    lcd.setCursor(8, 1);
    if (Iset >= 100)
  {
   lcd.print(Iset);
  }
  else if(Iset >=10)
  {
    lcd.print("    ");
    lcd.print(Iset);
  }
  else
  {
    lcd.print("     ");
    lcd.print(Iset);
  }
  lcd.setCursor(14, 1);
  lcd.print("mA");
 }
void Pshow()               //Se agregarn ceros y espacios necesarios para mostrar correctamente la corriente
{                              // y se la imprime por pantalla
    lcd.setCursor(8, 1);
       if (Pset >= 1000)
  { if(Pset<10000)
    {
     lcd.setCursor(11, 1);
    }
   else lcd.setCursor(10, 1);
    lcd.print(Pset/1000);
    lcd.setCursor(15, 1);
    lcd.print("W");
   
  }
  else if (Pset >=100)
  {
    lcd.print(" ");
    lcd.print(Pset);
    lcd.setCursor(14, 1);
    lcd.print("mW");
 
 }
 else{
    lcd.print("  ");
    lcd.print(Pset);
    lcd.setCursor(14, 1);
    lcd.print("mW");
 }
}

void Rshow()         //Se calcula la resistencia equivalente que tendría la carga
 {
   byte ohms[8] = {          //Se guarda el símbolo de Ohms
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B11011,
  B00000,
  };
  lcd.createChar(0, ohms);
if(Rset==10000)lcd.setCursor(10,1);
else{lcd.setCursor(11,1);}
      if(Rset<1000)
      {lcd.print(" ");
      if(Rset<100){lcd.print(" ");
      if(Rset<10)lcd.print(" ");}        //Se imprimen valores de R si es menor a 10k y se agregan espacios correspondientes
      }
      lcd.print(Rset);
      lcd.setCursor(15,1);
      lcd.write(byte(0));

   
 }
