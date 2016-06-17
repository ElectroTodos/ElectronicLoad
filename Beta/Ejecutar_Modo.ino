void RunMode()
{
  if(mode==0){
    noDisp=0;
    ShowTemp=0;
    Iset+=mult*encoder->getValue();
    if(Iset<0)Iset=0;
    if(Iset>3000)Iset=3000;
  }
  if(mode==1){
    noDisp=0;
    ShowTemp=3;
    Pset=Pset + mult*(encoder->getValue());
    if(Pset<0)Pset=0;
    if(Pset/1000>Pmax)Pset=Pmax;
    if(V!=0)Iset=Pset/(1000*V);
    else Iset=0;
    if(Iset<0)Iset=0;
    if(Iset>3000)Iset=3000;    
  }
  if(mode==2){
    noDisp=0;
    ShowTemp=4;
    Rset+=mult*(encoder->getValue()); 
    if(Rset>10000)Rset=10000;
    if(Rset<0)Rset=1;
    Iset=V/Rset;
    if(Iset<0)Iset=0;
    if(Iset>3000)Iset=3000;
  }
  if(mode==3){
   
    if(RampaNoConf==1){
      noDisp=1;
      RampaSetup();
      }
    else RunRampa();
  }
  }

