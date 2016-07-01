void RunMode()
{    
  if(V<Vcut){                                   //Se controla que la tension no sea inferior a la de corte
    Alerta=1;
    AlertaLatch=1;
  }                                             //Se obtienen los valores para despues setear la corriente
        
  if(mode==0){
    noDisp=0;
    Iset+=mult*encoder->getValue();
    if(Iset<0)Iset=0;
    if(Iset>3000)Iset=3000;
  }
  if(mode==1){
    noDisp=0;
    Pset=Pset + mult*(encoder->getValue());
    if(Pset<0)Pset=0;
    if(Pset/1000>Pmax)Pset=Pmax;
    if(V!=0)Iset=Pset/V;
    else Iset=0;
    if(Iset<0)Iset=0;
    if(Iset>3000)Iset=3000;    
  }
  if(mode==2){
    noDisp=0;
    Rset+=mult*(encoder->getValue()); 
    if(Rset>10000)Rset=10000;
    if(Rset<=0)Rset=10;
    Iset=1000*V/Rset;
    if(Iset<0)Iset=0;
    if(Iset>3000)Iset=3000;
  }
 
  if(mode==3){
  //  ShowTemp=5;
    noDisp=0;
    Vcut=Vcut+mult*(encoder->getValue());
    if(Vcut<0)Vcut=0;
    if(Vcut>50)Vcut=50;
  }
   if(mode==4){
   
    if(RampaNoConf==1){
      if(set==0){
        lcd.clear();
         set=1;
      }
      noDisp=1;
      RampaSetup();
      }
    else RunRampa();
  }
  }

