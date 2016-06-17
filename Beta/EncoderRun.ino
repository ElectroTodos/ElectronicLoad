void Encoder(){
    ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
     
     if(b == 5){                //Librería manda 5 al presionar
          if(mode == 0){
          if(mult==5)mult=50;
          else mult=5;          
     }
     if(mode == 1){
          if(mult==10)mult=500;
          else mult=10;
     }
     if(mode == 2){
      multmode++;
      if(multmode>3)multmode=0;
      switch(multmode){
        case 0:
          mult = 1;
          break;
        case 1: 
          mult = 10;
          break;
         case 2:
          mult = 100;
          break;
         case 3:
          mult = 1000;
          break;
      }
     }
     if(mode==3){
      multmode++;
      if(multmode>2)multmode=0;
      switch(multmode){
        case 0:
          if(mult==1)mult=60;
          else mult=1;
           break;
        case 1:
        case 2:
          if(mult==5)mult=50;
          else mult=5;
          break;
      }
      }
     }
     
     if(b == ClickEncoder::Held){    
           mode++;
                       if(mode>3)mode=0;
                       lcd.clear();
                       switch(mode){
                        case 0:
                            lcd.setCursor(1,0);
                            lcd.print("Corriente Cte.");
                            mult=5;
                           break;
                         case 1:
                            lcd.setCursor(2,0);
                            lcd.print("Potencia Cte.");
                            mult=10;
                            break;
                         case 2:
                            lcd.setCursor(0,0);
                            lcd.print("Resistencia Cte.");
                            Rset=10000;
                            mult=1;
                            multmode=0;
                            break;
                         case 3:
                            lcd.setCursor(4,0);
                            lcd.print("Rampa  /");
                            RampaNoConf=1;
                            mult=1;
                            lcd.clear();
                            Time=0;
                            Istart=0;
                            Istop=0;
                            break;
                        }
                        delay(500);
                        tiempo2=millis()+2000;
    }
    if(b == ClickEncoder::DoubleClicked){
           if(mode==3){
              selector++;
              if(selector>3)selector=0;
              if(selector==1)mult=5;
             }
           else{
             ShowTemp++;
             if(ShowTemp>2)ShowTemp=0;
             }
    }          
  }    
}












