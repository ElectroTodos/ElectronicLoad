void Display()
{
  if(tiempo2<=millis()){

   if(millis()>(tiempo+300))       //Delay para que se puedan apreciar los datos en el display                                   
   {    lcd.clear();               //Borramos lo que esté en el LCD  
         Tension();                //Imprimimos la tensión
        Corriente();               //Imprimimos la corriente
        switch(ShowTemp)
        {
          case 0:
               IsetShow();
              break;
          case 1:
               Resistencia();
              break;
          case 2:
              MostrarTemperatura();
              break;
          case 3:
               Pshow();
              break;
           case 4:
                Rshow();
              break;
           case 5:
                VcutShow();
                break;
           case 6:
                mAhShow();
                break;
           case 7:
                mWhShow();
                break; 
        }
      
         if((Alerta==1||AlertaLatch==1)&&(mode!=3))
        { 
          tone(4, 2000, 200);
          lcd.setCursor(0, 1);
          lcd.print("ALERTA");
        }
        else
        {
          Potencia();               //Imprimimos la potencia
        }
       
        tiempo=millis();
        
   }
  }
   Alerta=0;
}
