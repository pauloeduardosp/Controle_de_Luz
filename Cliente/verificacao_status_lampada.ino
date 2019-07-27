/* 
 * 
 * Verifica se houve alteração do status da lampada e envia up date ao server
 *     
 */

void verificacao_status_lampada(){
    now = millis();
    if(now - lastMeasureVerificacaoStatusLampada >= 600000) {
      verificacaoStatusLampada = true;
      lastMeasureVerificacaoStatusLampada = millis();
    }   
    for(int p = 0;p < qtdLuz; p++){
      if(tipoDispositivo[p]==1){
        if(digitalRead(gpioPrimario[p]) != lastStatusLampada[p] || verificacaoStatusLampada == true){
          lastStatusLampada[p] = digitalRead(gpioPrimario[p]);
          String st_value = mqttAtuadorId[p];
          char* c_topicoPublish = &st_value[0u];
          Serial.print("[STA] enviou up date - topicoPublish ");
          Serial.print(c_topicoPublish);
          Serial.print(" - valor ");
          if(digitalRead(gpioPrimario[p])){
            client.publish(c_topicoPublish, "on", 1);
            Serial.println(" on");
          } else {
            client.publish(c_topicoPublish, "off", 1);
            Serial.println(" off");
          }
        }
      }
    }
    verificacaoStatusLampada = false;

  
}
