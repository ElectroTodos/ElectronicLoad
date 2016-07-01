
/*    Firmware de ElectroTodos para Electronic Load
 *    En caso de dudas y/o consultas diígase a:  https://youtube.com/electronicatodos
 *    El proyecto se encuentra tanto en el canal antes mencionado.
 *    Decumentación adicional en: https://github.com/ElectroTodos/ElectronicLoad
 *    Se utilizan librerías externas bajo la tribución correspondiente.
 *    Relizado por Juan de ElectroTodos el 3/6/2016
 */
 
#include <Adafruit_MCP4725.h>         //Librería necesaria para el DAC. Para descargarla: https://github.com/adafruit/Adafruit_MCP4725
#include <ClickEncoder.h>             //Libreria necesaria para el rotary encoder. Para descargarla: https://github.com/0xPIT/encoder/tree/arduino
#include <TimerOne.h>                 //Libreria necesaria para el rotary encoder
#include <PID_v1.h>                   //Librería necesaria para el control del cooler. Para descargarla: http://playground.arduino.cc/Code/PIDLibrary
#include <Wire.h>                     //Librería necesaria para la comunicación I2C
#include <LiquidCrystal_I2C.h>        //Librería para el LCD I2C. Para descargarla: https://github.com/marcoschwartz/LiquidCrystal_I2C
LiquidCrystal_I2C lcd(0x27,16,2);     //Iniciamos el LCD 16x2
float I, V, P, A = 0, B, R,Pset,Rset=10000;    //Variables varias
float R1=1.1628,Max=36.15,Pmax=50,mult,T6;    //R1 es el valor real de la resistencia usada en el circuito. Pmax es la potencia máxima a disipar.
                                              //Max es el valor para el cual el divisor de tensión de la carga tendrá 5v a su salida
int Arranque=90,mode=0,ShowTemp,multmode,selector,Istart,Istop;                      //Arranque es el valor de PWM al que el cooler puede arrancar
float Temperatura,Iset,Vcut=0;                                                       //Variables varias
long double tiempo5;                                                                 //Tiempo con signo para la rampa
#define PIN_INPUT 2                                                                  //Entrada del sensor de temperatura
#define PIN_OUTPUT 3                                                                 //Salida PWM para el cooler
#define AT 75                                                                        //Temperatura a la que salta el alerta
#define der 8                                                                        //Pin del rotary encoder de giro hacia la derecha
#define izq 7                                                                        //Pin del rotary encoder de giro hacia la izquierda
#define pulsador 9                                                                   //Pin del rotary encoder del boton
#define Vcorr 1.118                                                                  //Correccion de la lectura de tension
#define Voff 0.13                                                                    //Correccion del offset de la tension
#define Icorr 1.074                                                                  //Correccion de la lectura de corriente
#define Ioff 13                                                                      //Correccion del offset de la corriente
unsigned long tiempo,tiempo2,tiempo4,Time,tiempo3;                                   //Tiempos varios
bool Alerta=0,noDisp=0,RampaNoConf=1,AlertaLatch,set;                                //Variables varias
double Setpoint, Input, Output;
double Kp=2, Ki=5, Kd=1;                                                             //Constantes del control PID de temperatura (no tocar a menos que se sepa lo que hacen)
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);                          //Configuración del PID
ClickEncoder *encoder;                                                               //Nombramos al encoder
int16_t last, value;
Adafruit_MCP4725 dac;                                                                //Nombramos al DAC
void timerIsr() {
  encoder->service();
}

                                      
void setup() {
  Serial.begin(9600);                                         //Iniciamos el serial
  pinMode(der,INPUT);                                         //Seteamos los pines de salida del encoder
  pinMode(izq,INPUT);
  pinMode(pulsador,INPUT);
  Timer1.initialize(1000);                                    //Configuramos un timer
  Timer1.attachInterrupt(timerIsr); 
  encoder = new ClickEncoder(der, izq, pulsador);             //Seteamos los pines para el encoder
  mult=5;
  dac.begin(0x60);                                            //Iniciamos el DAC
  Setpoint = 45;                                              //Temperatura a la que arranca el cooler (45)
  myPID.SetMode(AUTOMATIC);                                   //Le decimos al PID que esté en automático
  pinMode(5,INPUT);
  lcd.init();                                                 // Iniciamos todo lo necesario
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
  Cooler();                       //Se ajusta la velocidad del cooler
  Temp();                         //Leemos la temperatura
  Encoder();                      //Leemos el encoder
  RunMode();                      //Ejecutamos el modo que corresponda
  Cuentas();                      //Se realizan las mediciones y cuentas necesarias
  Dac();                          //Ajustamos la salida del DAC
  if(noDisp==0) Display();        //Imprimimos los diversos parámtros por pantalla
  serial();                       //Mandamos datos por serial
}

void Cuentas()
{
  int W;                          //Promediamos las entradas obtenidas para mejorar un poco los resultados
   for (W = 1; W <= 100; W++)
  {
    I = analogRead(0);
    A = A + I;
    V = analogRead(1);
    B = V + B;
  }
  I = A / 100;
  V = B / 100;
  if(I!=0) I =Ioff+Icorr* I * 1000 * 5 / (1024*R1);     //Calculamos la corriente en mA
  else I=0;
  if(V!=0) V =Voff+(Vcorr*V*Max/1024);                  //Calculamos la tensión en V
  else V=0;
  P = V * I / 1000;                                     //Calculamos la potencia en W
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
  Input = (5*analogRead(PIN_INPUT)*100)/1024;              //Se controla al cooler
  myPID.Compute();
  if(Output<Arranque){Output=0;}
  analogWrite(PIN_OUTPUT, Output);
}

void Temp()
{
  Temperatura=(5*analogRead(PIN_INPUT)*100)/1024;          //Se controla la temperatura
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
{                               //Se agregarn ceros y espacios necesarios para mostrar correctamente la temperatura
  byte grados[8] =              // y se la imprime por pantalla
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
  Serial.print(V);                 //Se imprimen los datos por serial
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

void IsetShow()                //Se agregarn ceros y espacios necesarios para mostrar correctamente la corriente
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
void Pshow()                   //Se agregarn ceros y espacios necesarios para mostrar correctamente la corriente
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

void Rshow()                 //Se calcula la resistencia equivalente que tendría la carga
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

void VcutShow(){                    //Se agregarn ceros y espacios necesarios para mostrar correctamente la tension seteada      
      lcd.setCursor(10,1);          // y se la imprime por pantalla
      if (Vcut >= 10)
        {lcd.print(Vcut);
          lcd.print("V");}
       else{
        lcd.print(" ");
        lcd.print(Vcut);
        lcd.print("V");   
       }
 }

