/*
 * 
 * Envio de update de informações para o server
 *
 */ 
void update_inicial(){

//##########################################################
//#### função parar de divulgar AP

  if(divulgaAP){
    if(millis() > 300000){
      if (WiFi.status() == WL_CONNECTED){
        WiFi.softAPdisconnect(true);
        divulgaAP = false;
        
        Serial.println("[Wifi_status] Parou de divulgar AP ");
        TelnetClient.println("[Wifi_status] Parou de divulgar AP ");
      }
    }
  }

//#### fim função parar de divulgar AP
//##########################################################


//##########################################################
//#### função envar update de status 

  if(wsStatus){
    if(delayversao < millis()){
      if(wsOldStatus == false){
          IPAddress ip = WiFi.localIP();
          idClient = ip[3];             // atribui o 4 octeto do ip ao idclient
          send_info() ; // envia informaçoes das lampadas
          for(int p = 0; p < qtdLuz ; p++){
            if(digitalRead(gpioLuz[p])){              // envia status atual da lampada
              webSocket.sendTXT(build_payload(socket_payload,0, idClient, gpioLuz[p], 0, 1),7);
              } else {
              webSocket.sendTXT(build_payload(socket_payload,0, idClient, gpioLuz[p], 0, 0),7);
              }
          }
          if(qtdup < 3){
            qtdup ++;
          } else {
            wsOldStatus = true;
          }
          
          Serial.println("[UpI] enviou up date");
          Serial.print("[UpI] IP WIFI ");
          Serial.println(ip);
      }
    }
  } else {
    wsOldStatus = false;
    qtdup = 0;
    delayversao = millis()+10000;
  }
}

//#### fim função envar update de status 
//##########################################################

void update_server(){

  Serial.println("[UpDate Server] Entou no update_server");
  if(server.arg("POST_updateServer") == "serverConectado"){
    Serial.println("[UpDate Server] Recebeu status de reconeccao do server");
    TelnetClient.println("[UpDate Server] Recebeu status de reconeccao do server");  
    wsOldStatus = false;  
    qtdup = 0;
  }

}
