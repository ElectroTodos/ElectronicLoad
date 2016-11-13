void TempControl(){

 if((millis()>(tiempo7+5000))||(PIN_OUTPUT==LOW))
  if (Temperatura>40)analogWrite(PIN_OUTPUT,200);  
  if(Temperatura>50)analogWrite(PIN_OUTPUT,255);
  if(Temperatura<=40)analogWrite(PIN_OUTPUT,0);
 tiempo7=millis();
}

