void Energia(){

  if(Reset==1){
    muestras=0;
    Itotal=0;
    Reset=0;
    Treset=millis();
    Serial.print("Entro");
  }
if(Wait==1){
  Treset=millis();
  Serial.print("Treset");
  }

if(Wait==0){
tiempo6=millis()-Treset;
muestras=muestras+1; 
Itotal=Itotal+I;
Vtot=Vtot+V; 
Iprom=Itotal/muestras;
Vprom=Vtot/muestras;
Pprom=Iprom*Vprom/1000; 
mAh=(Iprom*tiempo6)/3600000; 
mWh=(Pprom*tiempo6)/3600;
}
}

void mAhShow(){
      lcd.setCursor(7, 1);
    if (mAh >= 100)
  {
   lcd.print(mAh);
  }
  else if(mAh >=10)
  {
    lcd.print(" ");
    lcd.print(mAh);
  }
  else
  {
    lcd.print("  ");
    lcd.print(mAh);
  }
  lcd.setCursor(13, 1);
  lcd.print("mAh");
}

void mWhShow(){
      lcd.setCursor(7, 1);
    if (mWh >= 100)
  {
    if (mWh>=1000){
     lcd.setCursor(10, 1);
     lcd.print(mWh/1000);
     lcd.print("Wh");
     //
     //lcd.print("mWh");
    }
    else {
     lcd.print(mWh);
     lcd.setCursor(13, 1);
     lcd.print("mWh");
    }
  }
  else if(mWh >=10)
  {
    lcd.print(" ");
    lcd.print(mWh);
    lcd.setCursor(13, 1);
    lcd.print("mWh");
  }
  else
  {
    lcd.print("  ");
    lcd.print(mWh);
    lcd.setCursor(13, 1);
    lcd.print("mWh");
  }

}

