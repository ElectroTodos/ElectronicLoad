void Dac(){                                 //Se setea el valor del DAC
 float IsetTemp;
 if(Iset>3000)Iset=3000;
 if(Iset!=0)IsetTemp=Iset*1.08;
 else IsetTemp=Iset;
int dacIN= map(IsetTemp,0,5000,0,4095);
if (Alerta==1||AlertaLatch==1)dacIN=0;
dac.setVoltage(dacIN, false);
}
