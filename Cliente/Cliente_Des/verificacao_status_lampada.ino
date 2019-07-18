/* 
 * 
 * Verifica se houve alteração do status da lampada e envia up date ao server
 *     
 */

void verificacao_status_lampada(){

  for(int p = 0; p < qtdLuz;  p++){
      switch(tipoInterruptor[p]){
        case 1:   //  Paralelo
          if(digitalRead(gpioInterruptor[p]) != lastStatusLampada[p]){
          lastStatusLampada[p] = digitalRead(gpioInterruptor[p]);
            if(digitalRead(gpioInterruptor[p])) {
                webSocket.sendTXT(build_payload(socket_payload, 0, idServer[p], gpioLuz[p], 0, 1),7);
      
                Serial.printf("%d [STA] envou up date - lampada on\n", millis()/1000);
                TelnetClient.println("[STA] envou up date - lampada on");
            } else {
                webSocket.sendTXT(build_payload(socket_payload, 0, idServer[p], gpioLuz[p], 0, 0),7);
      
                Serial.printf("%d [STA] envou up date - lampada off", millis()/1000);
                TelnetClient.println("[STA] envou up date - lampada off");
            }    
          }
        break;
          
        case 2: //    Ligado ao GPIO

          if(digitalRead(gpioLuz[p]) != lastStatusLampada[p]){
            lastStatusLampada[p] = digitalRead(gpioLuz[p]);
            if(digitalRead(gpioLuz[p])){
              webSocket.sendTXT(build_payload(socket_payload, 0, idServer[p], gpioLuz[p], 0, 1),7);
    
              Serial.printf("%d [STA] envou up date - lampada on\n", millis()/1000);
              TelnetClient.println("[STA] envou up date - lampada on");
            } else {
              webSocket.sendTXT(build_payload(socket_payload, 0, idServer[p], gpioLuz[p], 0, 0),7);
    
              Serial.printf("%d [STA] envou up date - lampada off", millis()/1000);
              TelnetClient.println("[STA] envou up date - lampada off");
            }
          }
  
        break;
      }
  }    
}
