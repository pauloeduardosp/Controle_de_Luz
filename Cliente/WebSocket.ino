 
uint8_t * build_payload(uint8_t *payload, uint8_t control, uint8_t client, uint8_t gpio, uint8_t mode, uint8_t state){

  payload[0] = 0xaa;
  payload[1] = control;
  payload[2] = client;
  payload[3] = gpio;
  payload[4] = mode;
  payload[5] = state;
  payload[6] = 0x55;
  return payload;
}

void command_handle(uint8_t *payload) {

  if((payload[0]==0xaa) && (payload[1]==0x00) && (payload[6]==0x55)){           // alteracao de status da luz
    Serial.printf("idClient %d\n",idClient);
    if(payload[2]=idClient){
      switch (payload[4]) {
        case 0x00:    //mode 0 - lampada on/off
          if(payload[5]==0x03){
            digitalWrite(payload[3], !digitalRead(payload[3]));
            TelnetClient.println("[WSc] chegou comando");
          }
        break;

        case 0x01:    //mode 1 - reservado para PWM
          //PWM
        break;

        default:
        break;
      }
    }
  } else if((payload[0]==0xaa) && (payload[1]==0x03) && (payload[6]==0x55)){     // informaçao de idserver
    if(payload[2]==idClient){
      for(int p = 0; p < qtdLuz; p++){
        if(gpioLuz[p] == payload[3]){
          idServer[p] = payload[4];

          //###################################################
          // grava id do server na eeprom
          for(int p = EEPROM_idServer0, x = 0; p < (EEPROM_idServer0+qtdLuz); p++, x++){
            if(EEPROM.read(p)!= idServer[x]){                                    //valida se houve alteração na variavel antes de gravar 
                Serial.printf("[EEPROM SAVE] Atualizando idServer%d na eeprom para %d\n",x, idServer[x]);
                TelnetClient.printf("[EEPROM SAVE] Atualizando idLuz%d na eeprom para %d\n",x, idServer[x]);
                EEPROM.write(p, idServer[x]);                                      // grava variavel idserver na eeprom

                if(idServer[x] == 0){    // se recebeu id server = 0
                  wsOldStatus = false;  // muda wsOldStatus forçando reenvio de update
                  qtdup = 0;
                  Serial.println("[EEPROM SAVE] Mudou wsOldStatus para false");
                }
            }
          }
          EEPROM.commit();
          // fim
          //###################################################
         
          break;
        }
      }
    }
  } else if((payload[0]==0xaa) && (payload[1]==0x04) && (payload[6]==0x55)){     // wakeup server
    if(payload[2]==255){
      if(payload[5] == 1){
        for(int p = 0; p <= qtdLuz; p++){
          if(idServer[p] != 0){
            webSocket.sendTXT(build_payload(socket_payload, 4, idClient, 0, idServer[p], 2),7);  

            Serial.println("enviou status");
            Serial.printf("control: 4\n");
            Serial.printf("client: %d\n", idClient);
            Serial.printf("gpio: 0\n");
            Serial.printf("mode: %d\n",idServer[p]);
            Serial.printf("[WSc] state: 2\n");

          
          }
        }
      }
    }
    
  } else {
    Serial.printf("[WSc] Invalid command\n");
    TelnetClient.printf("[WSc] Invalid command\n");
  }
}


void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("%d\t[WSc] Disconnected!\n",millis()/1000);
            TelnetClient.printf("[WSc] Disconnected!\n");
            wsStatus = false;
            break;
        case WStype_CONNECTED:
            {
            wsStatus = true;
            Serial.printf("[WSc] Connected to url: %s\n",  payload);
            TelnetClient.printf("[WSc] Connected to url: %s\n",  payload);
            webSocket.sendTXT("Connected");
            }
            break;
        case WStype_TEXT:
            Serial.printf("\n[WSc] header: %d\n", payload[0]);
            TelnetClient.printf("\n[WSc] header: %d\n", payload[0]);
            Serial.printf("[WSc] control: %d\n", payload[1]);
            TelnetClient.printf("[WSc] control: %d\n", payload[1]);
            Serial.printf("[WSc] client: %d\n", payload[2]);
            TelnetClient.printf("[WSc] client: %d\n", payload[2]);
            Serial.printf("[WSc] gpio: %d\n", payload[3]);
            TelnetClient.printf("[WSc] gpio: %d\n", payload[3]);
            Serial.printf("[WSc] mode: %d\n", payload[4]);
            TelnetClient.printf("[WSc] mode: %d\n", payload[4]);
            Serial.printf("[WSc] state: %d\n", payload[5]);
            TelnetClient.printf("[WSc] state: %d\n", payload[5]);
            Serial.printf("[WSc] footer: %d\n", payload[6]);
            TelnetClient.printf("[WSc] footer: %d\n", payload[6]);
            Serial.printf("[WSc] get text: %s\n", payload);
            TelnetClient.printf("[WSc] get text: %s\n", payload);
            command_handle(payload);
            break;
        case WStype_BIN:
            Serial.printf("[WSc] get binary lenght: %u\n", lenght);
            TelnetClient.printf("[WSc] get binary lenght: %u\n", lenght);
            hexdump(payload, lenght);
            break;
    }
}
