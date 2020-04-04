/*
 *  
 * Verifica mudança de status do interruptor
 *     
 */
void mudanca_interruptor(){

  for(int p = 0; p < qtdLuz; p++){
    if (tipoInterruptor[p] == 2){
      if (digitalRead(gpioSecundario[p]) != lastStatusInt[p]){
        if(delayinterruptor < millis()){
          digitalWrite(gpioPrimario[p], !digitalRead(gpioPrimario[p]));
          Serial.println("[MI] mudança de status interruptor");
            
          lastStatusInt[p] = digitalRead(gpioSecundario[p]);
          Serial.printf("[MI] Pino %d mudou para %d\n",gpioSecundario[p], digitalRead(gpioSecundario[p]));
          delayinterruptor=millis()+1000;
        }
      }
    }
  }
}
