/* 
 * 
 * Verifica se houve alteração do status da lampada e envia up date ao server
 *     
 */

void verificacao_status_lampada(){

    for(int p = 0;p < qtdLuz; p++){
      if(digitalRead(gpioLuz[p]) != lastStatusLampada[p]){
        lastStatusLampada[p] = digitalRead(gpioLuz[p]);
        if(digitalRead(gpioLuz[p])){
//          webSocket.sendTXT(build_payload(socket_payload, 0, idServer[p], gpioLuz[p], 0, 1),7);
          TelnetClient.println("[STA] envou up date - lampada on");
        } else {
//          webSocket.sendTXT(build_payload(socket_payload, 0, idServer[p], gpioLuz[p], 0, 0),7);
          TelnetClient.println("[STA] envou up date - lampada off");
        }
      }
    }

  
}
