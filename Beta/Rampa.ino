void RampaSetup(){
  Iset=0;
  switch(selector){
    case 0:
        Time+=mult*encoder->getValue();
        if(Time<=0)Time=60;
        if(Time>3600)Time=3600;
       break;
     case 1:
         Istart+=mult*encoder->getValue();
         if(Istart<0)Istart=0;
         if(Istart>3000)Istart=3000;
       break;
     case 2:
         Istop+=mult*encoder->getValue();
         if(Istop<0)Istop=0;
         if(Istop>3000)Istop=3000;
       break;
       case 3:
        RampaNoConf=0;
        TimeD=Time;
       // tiempo3=(millis()+1000*TimeD);
       // Serial.print(float(tiempo3));
        
        break;
       
  }
  if(millis()>(tiempo+300)){  
  lcd.setCursor(0,0);
  lcd.print("Rampa");
  lcd.setCursor(10,0);
  lcd.print(Time);
  lcd.print("S");
  lcd.setCursor(0,1);
  lcd.print(Istart);
  lcd.print("mA");
  lcd.setCursor(10,1);
  lcd.print(Istop);
  lcd.print("mA");
  tiempo=millis();
  }
}

void RunRampa(){
  tiempo4=millis();
  if(selector=3)tiempo3=(tiempo4+1000*TimeD);
 ShowTemp=0;
noDisp=0;
 // Serial.print("\t");
   if(tiempo4<tiempo3){
    Serial.print(Istart);
    Serial.print("\t");
  //   Istop=100.0;
  //   Istart=5.0;
  T6=tiempo4/tiempo3;
  Serial.print(T6);
    Serial.print("\t");
  IsetD=(Istop-Istart)*T6+Istart;
  Iset=IsetD;
  Serial.print(Istop);
     Serial.print("\t");
  if(Iset>3000)Iset=3000;}
  else{
    Iset=0;
    RampaNoConf=1;
    selector=0;
    lcd.clear();
  }
}

