/* 
 * 
 * manipulação da eeprom
 *  
 */  
 
void saveConfig() {
  Serial.println("[EEPROM SAVE] Entrou na funcao saveConfig()");

  boolean reset = false;

  // ##################### Configuração WIFI SSID e Password
  struct temp{                                          // gera variavel temp para validar dados da 
    char ssid[30]; 
    char password[20];
  } temp;
  
  for(unsigned int p=0; p <=  sizeof(wifi); p++){   
    *((char*)&temp + p) = EEPROM.read(EEPROM_wifi + p);   // grava dados do wifi na variavel temp
  }



  if( (String(temp.ssid) != String(wifi.ssid)) ||
      (String(temp.password) != String(wifi.password))) {             // valida se houve alteração na variavel wifi antes de gravar 
    reset = true;
    Serial.printf("[EEPROM] Atualizando wifi.ssid na eeprom para "); 
    Serial.println(String(wifi.ssid));
    Serial.printf("[EEPROM] Atualizando wifi.password na eeprom para "); 
    Serial.println(String(wifi.password));
    for(unsigned int p=0; p <=  sizeof(wifi); p++){
      EEPROM.write(EEPROM_wifi + p, *((char*)&wifi + p)); // grava dados da variavel wifi.ssid na eeprom
    }    
  } 


   // ##################### mqttServer

  char temp_mqttServer[40];
  for(unsigned int p=0; p <=  sizeof(mqttServer); p++){   
    *((char*)&temp_mqttServer + p) = EEPROM.read(EEPROM_mqttServer + p);    // grava dados EEPROM_mqttServer na variavale temp_mqttServer
  }
  char char_mqttServer[40];
  mqttServer.toCharArray(char_mqttServer, 40);                              // converte string mqttServer em charArray 
//  mqttServer = char_mqttServer;
  if(String(temp_mqttServer) != mqttServer){                                // compara se houve alteraçao na variavel mqttServer
//    reset = true;
    Serial.print("[EEPROM SAVE] Atualizando mqttServer na eeprom para ");
    Serial.println(mqttServer);
    for(unsigned int p=0; p <= mqttServer.length(); p++){
      EEPROM.write(EEPROM_mqttServer + p, *((char*)&char_mqttServer + p));  // grava novo dado na eemprom
    }   
  }

  // ##################### Config Device
  // ##################### QtdLuz

  if(EEPROM.read(EEPROM_qtdLuz)!= qtdLuz){                                    //valida se houve alteração na variavel antes de gravar 
    reset = true;
    Serial.printf("[EEPROM SAVE] Atualizando qtdLuz na eeprom para %d\n",qtdLuz);
    EEPROM.write(EEPROM_qtdLuz, qtdLuz);                                      // grava variavel qtdluz na eeprom
  } 



  // ##################### GPIO
   
    for(int p = EEPROM_gpioPrimario0, x=0; p < (EEPROM_gpioPrimario0+qtdLuz); p++, x++){
      if(EEPROM.read(p)!= gpioPrimario[x]){                                    //valida se houve alteração na variavel antes de gravar 
          reset = true;
          Serial.printf("[EEPROM SAVE] Atualizando gpioPrimario%d na eeprom para %d\n",x, gpioPrimario[x]);
          EEPROM.write(p, gpioPrimario[x]);                                      // grava variavel gpioPrimario na eeprom
      }
    }
  

  // ##################### mqttAtuadorId
    int total = EEPROM_mqttAtuadorId0 + 40*qtdLuz;
    for(int x = EEPROM_mqttAtuadorId0, y=0; x < total; x += 40, y++){
      char temp_mqttAtuadorId[40];
      for(unsigned int p=0; p <=  sizeof(mqttAtuadorId[y]); p++){   
        *((char*)&temp_mqttAtuadorId + p) = EEPROM.read(x + p);    // grava dados EEPROM_nomeDevice na variavale temp_nomeDevice
      }
    
      char char_mqttAtuadorId[40];
      mqttAtuadorId[y].toCharArray(char_mqttAtuadorId, 40);                              // converte string nomeDevice em charArray 
      if(String(temp_mqttAtuadorId) != mqttAtuadorId[y]){                                // compara se houve alteraçao na variavel nomeDevice
        Serial.printf("[EEPROM SAVE] Atualizando mqttAtuadorId%d na eeprom para ",y);
        Serial.println(mqttAtuadorId[y]);
        for(unsigned int p=0; p <= mqttAtuadorId[y].length(); p++){
          EEPROM.write(x + p, *((char*)&char_mqttAtuadorId + p));  // grava novo dado na eemprom
         }
        
      }
    }


  // ##################### Configuração Interruptor
  // ##################### GPIO
      for(int p = EEPROM_gpioSecundario0, x=0; p < (EEPROM_gpioSecundario0+qtdLuz); p++, x++){
        if(EEPROM.read(p)!= gpioSecundario[x]){                                    //valida se houve alteração na variavel antes de gravar 
          reset = true;
          Serial.printf("[EEPROM SAVE] Atualizando gpioSecundario%d na eeprom para %d\n",x, gpioSecundario[x]);
        
          EEPROM.write(p, gpioSecundario[x]);                                      // grava variavel gpioPrimario na eeprom
        }
      }

  // ##################### Tipo  
      String value;
      for(int p = EEPROM_tipoInterruptor0, x=0; p < (EEPROM_tipoInterruptor0+qtdLuz); p++, x++){
        if(EEPROM.read(p)!= tipoInterruptor[x]){                                    //valida se houve alteração na variavel antes de gravar 
          switch(tipoInterruptor[p]){
            case 1:
              value = "Paralelo";
              Serial.println(" interruptor paralelo ");
              Serial.println(value);
              break;
            
            case 2:
              value = "Ligado ao GPIO";
              Serial.println(" interruptor gpio ");
              Serial.println(value);
              break;
          }          
          Serial.printf("[EEPROM SAVE] Atualizando tipoInterruptor%d na eeprom para %d - ",x, tipoInterruptor[x]); 
          Serial.println(value);
          EEPROM.write(p, tipoInterruptor[x]);                                      // grava variavel gpioPrimario na eeprom
        }
      }




  // ##################### Tipo  Dispositivo
         for(int p = EEPROM_tipoDispositivo0, x=0; p < (EEPROM_tipoDispositivo0+qtdLuz); p++, x++){
        if(EEPROM.read(p)!= tipoDispositivo[x]){                                    //valida se houve alteração na variavel antes de gravar 
          switch(tipoDispositivo[p]){
            case 1:
              value = "Luz";
//              Serial.println(" Disposivito ");
//              Serial.println(value);
              break;
            
            case 2:
              value = "dht";
//              Serial.println(" Dispositivo ");
//              Serial.println(value);
              break;
          }          
          Serial.printf("[EEPROM SAVE] Atualizando tipoDispositivo%d na eeprom para %d - ",x, tipoInterruptor[x]); 
          Serial.println(value);
          EEPROM.write(p, tipoDispositivo[x]);                                      // grava variavel gpioPrimario na eeprom
        }
      }



    EEPROM.commit();
    
    if(reset == true){
      Serial.println("[EEPROM SAVE] Realizando reset no modulo");
      ESP.reset(); 
    }
//  }
    
}



void loadConfig(){

  
  Serial.printf("\n[EEPROM LOAD] Entrou na funcao loadConfig()\n");

// ##################### Wifi SSID e Passowrd
  if((EEPROM.read(EEPROM_wifi) != 255) && (EEPROM.read(EEPROM_wifi+1) != 255)){     // verifica se a eprom é diferente de 255
    for(unsigned int p=0; p <=  sizeof(wifi); p++){
      *((char*)&wifi + p) = EEPROM.read(EEPROM_wifi + p);                           // le variavel da eeprom
    }
  }
    Serial.printf("[EEPROM LOAD] Atualizando wifi.ssid para "); 
    Serial.println(String(wifi.ssid));
    Serial.printf("[EEPROM LOAD] Atualizando wifi.password para "); 
    Serial.println(String(wifi.password));
 

// ##################### Mqtt Server

  if(EEPROM.read(EEPROM_mqttServer) != 255){ 
    char char_mqttServer[40];
    for(unsigned int p=0; p <=  40; p++){   
      *((char*)&char_mqttServer + p) = EEPROM.read(EEPROM_mqttServer + p);   // grava dados EEPROM_mqttServer na variavale temp_mqttServer

    }
    mqttServer = char_mqttServer;
    Serial.printf("[EEPROM LOAD] Atualizando mqttServer para ");
    Serial.println(mqttServer);
    
  }

// ##################### QtdLuz

  if(EEPROM.read(EEPROM_qtdLuz) >= 1){
    if(EEPROM.read(EEPROM_qtdLuz) <= 4){
      qtdLuz = EEPROM.read(EEPROM_qtdLuz);                                // le variavel da eeprom
      Serial.printf("[EEPROM LOAD] Atualizando qtdLuz para %d\n",qtdLuz);
    }
  }


// ##################### Configuração Luz
// ##################### GPIO
  for(int p = EEPROM_gpioPrimario0, x=0; p < (EEPROM_gpioPrimario0+qtdLuz); p++, x++){
    if(EEPROM.read(p) >= 1){
      if(EEPROM.read(p) <= 50){
        gpioPrimario[x] = EEPROM.read(p);                                // le variavel da eeprom
        Serial.printf("[EEPROM LOAD] Atualizando gpioPrimario%d para %d\n",x, gpioPrimario[x]);
      }
    }
  }


// ##################### mqttAtuadorId
  int total = EEPROM_mqttAtuadorId0 + 40*qtdLuz;
  for(int x = EEPROM_mqttAtuadorId0, y=0; x < total; x += 40, y++){
     if(EEPROM.read(x) != 255){ 
      char char_mqttAtuadorId[40];
      for(unsigned int p=0; p <=  40; p++){   
        *((char*)&char_mqttAtuadorId + p) = EEPROM.read(x + p);   // grava dados EEPROM_mqttAtuadorId0 na variavale temp_mqttAtuadorId
      }
      mqttAtuadorId[y] = String(char_mqttAtuadorId);
      Serial.printf("[EEPROM LOAD] Atualizando mqttAtuadorId%d para ",y); 
      Serial.println(mqttAtuadorId[y]);

    }
  }


// ##################### Configuração Interruptor
//// ##################### GPIO
  for(int p = EEPROM_gpioSecundario0, x=0; p < (EEPROM_gpioSecundario0+qtdLuz); p++, x++){
    if(EEPROM.read(p) >= 0){
      if(EEPROM.read(p) <= 50){
        gpioSecundario[x] = EEPROM.read(p);                                // le variavel da eeprom
        Serial.printf("[EEPROM LOAD] Atualizando gpioSecundario%d para %d\n",x, gpioSecundario[x]);
      }
    }
  }

  for(int p = EEPROM_tipoInterruptor0, x=0; p < (EEPROM_tipoInterruptor0+qtdLuz); p++, x++){
    if(EEPROM.read(p) >= 1){
      if(EEPROM.read(p) <= 50){
        tipoInterruptor[x] = EEPROM.read(p);                                // le variavel da eeprom
        Serial.printf("[EEPROM LOAD] Atualizando tipoInterruptor%d para %d\n",x, tipoInterruptor[x]);
      }
    }
  }

// ##################### Configuração Tipo Dispositivo
  for(int p = EEPROM_tipoDispositivo0, x=0; p < (EEPROM_tipoDispositivo0+qtdLuz); p++, x++){
    if(EEPROM.read(p) >= 1){
      if(EEPROM.read(p) <= 50){
        tipoDispositivo[x] = EEPROM.read(p);                                // le variavel da eeprom
        Serial.printf("[EEPROM LOAD] Atualizando tipoDispositivo%d para %d\n",x, tipoInterruptor[x]);
      }
    }
  }

}
