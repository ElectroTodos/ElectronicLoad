void Dac(){
int dacIN= 1.17*map(Iset,0,5000,0,4095);
if (Alerta==1)dacIN=0;
dac.setVoltage(dacIN, false);
}
