void RampaSetup(){
  Iset=0;
  switch(selector){
    case 0:
        Time+=mult*encoder->getValue();
        if(Time<=0)Time=30;
        if(Time>600)Time=600;
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
       tiempo3=millis();
        break;
       
  }
  if(millis()>(tiempo+300)){  
  lcd.setCursor(0,0);
  lcd.print("Rampa");
  lcd.setCursor(12,0);
  if(Time<100)lcd.print(" ");
  if(Time<10) lcd.print(" ");
  lcd.print(Time);
  lcd.print("S");
  lcd.setCursor(0,1);
  if(Istart<1000)lcd.print(" ");
  if(Istart<100)lcd.print(" ");
  if(Istart<10) lcd.print(" ");
  lcd.print(Istart);
  lcd.print("mA");
  lcd.setCursor(10,1);
  if(Istop<1000)lcd.print(" ");
  if(Istop<100)lcd.print(" ");
  if(Istop<10) lcd.print(" ");
  lcd.print(Istop);
  lcd.print("mA");
  tiempo=millis();
  }
}

void RunRampa(){
tiempo4=millis()-tiempo3;
tiempo5=tiempo4;
ShowTemp=0;
noDisp=0;
   if(tiempo4<1000*Time){
      Iset=(Istop-Istart)*tiempo5/(1000*Time)+Istart;
      if(Iset>3000)Iset=3000;
  }
  else{
    Iset=0;
    RampaNoConf=1;
    selector=0;
    lcd.clear();
    set=0;
   }
}
