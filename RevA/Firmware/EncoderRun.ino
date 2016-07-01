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
     if(mode==4){
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
     if(mode==3){
       multmode++;
      if(multmode>1)multmode=0;
      switch(multmode){
        case 0:
          mult = 0.05;
          break;
        case 1: 
          mult = 1;
          break;
         }
     }
     }
     
     if(b == ClickEncoder::Held){    
           mode++;
                       if(mode>4)mode=0;                                  //Se cambian los modos segun se presione el encoder
                       lcd.clear();
                       switch(mode){
                        case 0:
                            lcd.setCursor(1,0);
                            lcd.print("Corriente Cte.");
                            mult=5;
                            Iset=0;
                            AlertaLatch=0;
                            ShowTemp=0;
                           break;
                         case 1:
                            lcd.setCursor(2,0);
                            lcd.print("Potencia Cte.");
                            Pset=0;
                            mult=10;
                            AlertaLatch=0;
                            ShowTemp=3;
                            break;
                         case 2:
                            lcd.setCursor(0,0);
                            lcd.print("Resistencia Cte.");
                            Rset=10000;
                            mult=1;
                            multmode=0;
                            AlertaLatch=0;
                            ShowTemp=4;
                            break;
                         case 4:
                            AlertaLatch=0;
                            lcd.setCursor(4,0);
                            lcd.print("Rampa  /");
                            RampaNoConf=1;
                            mult=1;
                            Time=30;
                            Istart=0;
                            Istop=0;
                            break;
                          case 3:
                            Iset=0;
                            lcd.setCursor(2,0);
                            lcd.print("Tension de");
                            lcd.setCursor(5,1);
                            lcd.print("Corte");
                            mult=0.05;
                            multmode=0;
                            if(AlertaLatch=1){
                              Alerta=0;
                              AlertaLatch=0;
                            }
                            ShowTemp=5;
                            set=0;
                            break;
                        }
                        delay(500);
                        tiempo2=millis()+2000;
    }
    if(b == ClickEncoder::DoubleClicked){
           if(mode==4){
              selector++;
              if(selector>3)selector=0;
              if(selector==1)mult=5;
             }
           else{
             ShowTemp++;
             Serial.print(ShowTemp);
             Serial.print("\n");
             if(ShowTemp>5)ShowTemp=0;
             }
    }             
  }
}












