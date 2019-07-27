/*
 *  
 * Verifica mudança de status do interruptor
 *     
 */
void mudanca_interruptor(){

  for(int p = 0; p < qtdLuz; p++){
    if (digitalRead(gpioSecundario[p]) != lastStatusInt[p]){
  //  if(delayinterruptor<millis()){
  //   if (digitalRead(gpioSecundario[p]) != lastStatusInt[p]){
        digitalWrite(gpioPrimario[p], !digitalRead(gpioPrimario[p]));
        Serial.println("[MI] mudança de status interruptor");
        lastStatusInt[p] = digitalRead(gpioSecundario[p]);
        delayinterruptor=millis()+800;
//        delay(750);
        }
     }
//  }
}
