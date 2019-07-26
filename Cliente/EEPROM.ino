/* 
 * 
 * manipulação da eeprom
 *  
 */  
 
void saveConfig() {
  Serial.println("[EEPROM SAVE] Entrou na funcao saveConfig()");
  TelnetClient.println("[EEPROM SAVE] Entrou na funcao saveConfig()");

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
    TelnetClient.printf("[EEPROM] Atualizando wifi ssid na eeprom para "); 
    TelnetClient.println(String(wifi.ssid));
    TelnetClient.printf("[EEPROM] Atualizando wifi.password na eeprom para "); 
    TelnetClient.println(String(wifi.password));
    for(unsigned int p=0; p <=  sizeof(wifi); p++){
      EEPROM.write(EEPROM_wifi + p, *((char*)&wifi + p)); // grava dados da variavel wifi.ssid na eeprom
    }    
  } 


  // ##################### Configuração IpServer
  Serial.println("// ##################### Configuração IpServer");
  for(byte p = 0; p < 4; p++){
    if(octeto[p] != EEPROM.read(EEPROM_octeto0 + p)){
      EEPROM.write(EEPROM_octeto0 + p, octeto[p]);
      reset = true;
    }
  }

  // ##################### Configuração Luz
  // ##################### Nome Device

  char temp_nomeDevice[20];
  for(unsigned int p=0; p <=  sizeof(nomeDevice); p++){   
    *((char*)&temp_nomeDevice + p) = EEPROM.read(EEPROM_nomeDevice + p);    // grava dados EEPROM_nomeDevice na variavale temp_nomeDevice
  }
  char char_nomeDevice[20];
  nomeDevice.toCharArray(char_nomeDevice, 20);                              // converte string nomeDevice em charArray 
  if(String(temp_nomeDevice) != nomeDevice){                                // compara se houve alteraçao na variavel nomeDevice
    Serial.print("[EEPROM SAVE] Atualizando nomeDevice na eeprom para ");
    Serial.println(nomeDevice);
    TelnetClient.print("[EEPROM SAVE] Atualizando nomeDevice na eeprom para ");
    TelnetClient.println(nomeDevice);
    for(unsigned int p=0; p <= sizeof(nomeDevice); p++){
      EEPROM.write(EEPROM_nomeDevice + p, *((char*)&char_nomeDevice + p));  // grava novo dado na eemprom
    }
    
  }

  // ##################### QtdLuz

  if(EEPROM.read(EEPROM_qtdLuz)!= qtdLuz){                                    //valida se houve alteração na variavel antes de gravar 
    reset = true;
    Serial.printf("[EEPROM SAVE] Atualizando qtdLuz na eeprom para %d\n",qtdLuz);
    TelnetClient.printf("[EEPROM SAVE] Atualizando qtdLuz na eeprom para %d\n",qtdLuz);
    EEPROM.write(EEPROM_qtdLuz, qtdLuz);                                      // grava variavel qtdluz na eeprom
  } 
 
  // ##################### Configuração Luz
  // ##################### ID
  
    for(int p = EEPROM_idLuz0, x = 0; p < (EEPROM_idLuz0+qtdLuz); p++, x++){
      if(EEPROM.read(p)!= idLuz[x]){                                    //valida se houve alteração na variavel antes de gravar 
          Serial.printf("[EEPROM SAVE] Atualizando idLuz%d na eeprom para %d\n",x, idLuz[x]);
          TelnetClient.printf("[EEPROM SAVE] Atualizando idLuz%d na eeprom para %d\n",x, idLuz[x]);
          EEPROM.write(p, idLuz[x]);                                      // grava variavel idLuz na eeprom
      }
    }


  // ##################### IdServer

  for(int p = EEPROM_idServer0, x = 0; p < (EEPROM_idServer0+qtdLuz); p++, x++){
    if(EEPROM.read(p)!= idServer[x]){                                    //valida se houve alteração na variavel antes de gravar 
        Serial.printf("[EEPROM SAVE] Atualizando idServer%d na eeprom para %d\n",x, idServer);
        TelnetClient.printf("[EEPROM SAVE] Atualizando idServer%d na eeprom para %d\n",x, idServer);
        EEPROM.write(p, idServer[x]);                                      // grava variavel gpioLuz na eeprom
    }
  }
  
  // ##################### GPIO
   
    for(int p = EEPROM_gpioLuz0, x=0; p < (EEPROM_gpioLuz0+qtdLuz); p++, x++){
      if(EEPROM.read(p)!= gpioLuz[x]){                                    //valida se houve alteração na variavel antes de gravar 
          reset = true;
          Serial.printf("[EEPROM SAVE] Atualizando gpioLuz%d na eeprom para %d\n",x, gpioLuz[x]);
          TelnetClient.printf("[EEPROM SAVE] Atualizando gpioLuz%d na eeprom para %d\n",x, gpioLuz[x]);
          EEPROM.write(p, gpioLuz[x]);                                      // grava variavel gpioLuz na eeprom
      }
    }
  
  // ##################### Nome Luz
    int total = EEPROM_nomeLuz0 + 20*qtdLuz;
    for(int x = EEPROM_nomeLuz0, y=0; x < total; x += 20, y++){
      char temp_nomeLuz[20];
      for(unsigned int p=0; p <=  sizeof(nomeLuz[y]); p++){   
        *((char*)&temp_nomeLuz + p) = EEPROM.read(x + p);    // grava dados EEPROM_nomeDevice na variavale temp_nomeDevice
      }
    
      char char_nomeLuz[20];
      nomeLuz[y].toCharArray(char_nomeLuz, 20);                              // converte string nomeDevice em charArray 
      if(String(temp_nomeLuz) != nomeLuz[y]){                                // compara se houve alteraçao na variavel nomeDevice
        Serial.printf("[EEPROM SAVE] Atualizando nomeLuz%d na eeprom para ",y);
        Serial.println(nomeLuz[y]);
        TelnetClient.printf("[EEPROM SAVE] Atualizando nomeLuz%d na eeprom para ",y);
        TelnetClient.println(nomeLuz[y]);
        for(unsigned int p=0; p <= sizeof(nomeLuz[y]); p++){
          EEPROM.write(x + p, *((char*)&char_nomeLuz + p));  // grava novo dado na eemprom
        }
        
      }
    }
  
  // ##################### Configuração Interruptor
  // ##################### GPIO
      for(int p = EEPROM_gpioInterruptor0, x=0; p < (EEPROM_gpioInterruptor0+qtdLuz); p++, x++){
        if(EEPROM.read(p)!= gpioInterruptor[x]){                                    //valida se houve alteração na variavel antes de gravar 
          reset = true;
          Serial.printf("[EEPROM SAVE] Atualizando gpioInterruptor%d na eeprom para %d\n",x, gpioInterruptor[x]);
          TelnetClient.printf("[EEPROM SAVE] Atualizando gpioInterruptor%d na eeprom para %d\n",x, gpioInterruptor[x]);
          EEPROM.write(p, gpioInterruptor[x]);                                      // grava variavel gpioLuz na eeprom
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
          TelnetClient.printf("[EEPROM SAVE] Atualizando tipoInterruptor%d na eeprom para %d - ",x, tipoInterruptor[x]); 
          TelnetClient.println(value);
          EEPROM.write(p, tipoInterruptor[x]);                                      // grava variavel gpioLuz na eeprom
        }
      }

    EEPROM.commit();
    
    if(reset == true){
      Serial.println("[EEPROM SAVE] Realizando reset no modulo");
      TelnetClient.println("[EEPROM SAVE] Realizando reset no modulo");
      ESP.restart();  
    }
//  }
    
}



void loadConfig(){
  Serial.println("[EEPROM LOAD] Entrou na funcao loadConfig()");
  TelnetClient.println("[EEPROM LOAD] Entrou na funcao loadConfig()");


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
    TelnetClient.printf("[EEPROM LOAD] Atualizando wifi ssid para "); 
    TelnetClient.println(String(wifi.ssid));
    TelnetClient.printf("[EEPROM LOAD] Atualizando wifi.password para "); 
    TelnetClient.println(String(wifi.password));

// ##################### IpServer
  for(byte p = 0; p < 4; p++){
    octeto[p] = EEPROM.read(EEPROM_octeto0 + p);
    Serial.printf("[EEPROM LOAD] Atualizando octeto[%d] para %d \n", p, octeto[p]);
    TelnetClient.printf("[EEPROM LOAD] Atualizando octeto[%d] para %d \n", p, octeto[p]);
  }


// ##################### Nome Device

  if(EEPROM.read(EEPROM_nomeDevice) != 255){ 
    char char_nomeDevice[20];
    for(unsigned int p=0; p <=  sizeof(nomeDevice); p++){   
      *((char*)&char_nomeDevice + p) = EEPROM.read(EEPROM_nomeDevice + p);   // grava dados EEPROM_nomeDevice na variavale temp_nomeDevice

    }
    nomeDevice = String(char_nomeDevice);
    Serial.printf("[EEPROM LOAD] Atualizando nomeDevice para ");
    Serial.println(nomeDevice);
    TelnetClient.printf("[EEPROM LOAD] Atualizando nomeDevice para ");
    TelnetClient.println(nomeDevice);
  }

// ##################### QtdLuz

  if(EEPROM.read(EEPROM_qtdLuz) >= 1){
    if(EEPROM.read(EEPROM_qtdLuz) <= 4){
      qtdLuz = EEPROM.read(EEPROM_qtdLuz);                                // le variavel da eeprom
      Serial.printf("[EEPROM LOAD] Atualizando qtdLuz para %d\n",qtdLuz);
      TelnetClient.printf("[EEPROM LOAD] Atualizando qtdLuz para %d\n",qtdLuz);
    }
  }


// ##################### Configuração Luz
// ##################### ID

  for(int p = EEPROM_idLuz0, x=0; p < (EEPROM_idLuz0+qtdLuz); p++, x++){
    if(EEPROM.read(p) >= 1){
      if(EEPROM.read(p) <= 50){
        idLuz[x] = EEPROM.read(p);                                // le variavel da eeprom
        Serial.printf("[EEPROM LOAD] Atualizando idLuz%d para %d\n",x, idLuz[x]);
        TelnetClient.printf("[EEPROM LOAD] Atualizando idLuz%d para %d\n",x, idLuz[x]);
      }
    }
  }

// ##################### IdServer
  for(int p = EEPROM_idServer0, x=0; p < (EEPROM_idServer0+qtdLuz); p++, x++){
    if(EEPROM.read(p)  != 255){ 
      idServer[x] = EEPROM.read(p);                    // le variavel da eeprom
      Serial.printf("[EEPROM LOAD] Atualizando idServer%d para %d\n", x, idServer[x]);
      TelnetClient.printf("[EEPROM LOAD] Atualizando idServer%d para %d\n", x, idServer[x]);
    }
  }
  
// ##################### GPIO
  for(int p = EEPROM_gpioLuz0, x=0; p < (EEPROM_gpioLuz0+qtdLuz); p++, x++){
    if(EEPROM.read(p) >= 1){
      if(EEPROM.read(p) <= 50){
        gpioLuz[x] = EEPROM.read(p);                                // le variavel da eeprom
        Serial.printf("[EEPROM LOAD] Atualizando gpioLuz%d para %d\n",x, gpioLuz[x]);
        TelnetClient.printf("[EEPROM LOAD] Atualizando gpioLuz%d para %d\n",x, gpioLuz[x]);
      }
    }
  }

// ##################### Nome Luz
  int total = EEPROM_nomeLuz0 + 20*qtdLuz;
  for(int x = EEPROM_nomeLuz0, y=0; x < total; x += 20, y++){
    
    if(EEPROM.read(x) != 255){ 
      char char_nomeLuz[20];
      for(unsigned int p=0; p <=  sizeof(nomeLuz[0]); p++){   
        *((char*)&char_nomeLuz + p) = EEPROM.read(x + p);   // grava dados EEPROM_nomeLuz0 na variavale temp_nomeLuz
      }
      nomeLuz[y] = String(char_nomeLuz);
      Serial.printf("[EEPROM LOAD] Atualizando nomeLuz%d para ",y); 
      Serial.println(nomeLuz[y]);
      TelnetClient.printf("[EEPROM LOAD] Atualizando nomeLuz%d para ",y); 
      TelnetClient.println(nomeLuz[y]);
    }
  }


// ##################### Configuração Interruptor
//// ##################### GPIO
  for(int p = EEPROM_gpioInterruptor0, x=0; p < (EEPROM_gpioInterruptor0+qtdLuz); p++, x++){
    if(EEPROM.read(p) >= 0){
      if(EEPROM.read(p) <= 50){
        gpioInterruptor[x] = EEPROM.read(p);                                // le variavel da eeprom
        Serial.printf("[EEPROM LOAD] Atualizando gpioInterruptor%d para %d\n",x, gpioInterruptor[x]);
        TelnetClient.printf("[EEPROM LOAD] Atualizando gpioInterruptor%d para %d\n",x, gpioInterruptor[x]);
      }
    }
  }

  for(int p = EEPROM_tipoInterruptor0, x=0; p < (EEPROM_tipoInterruptor0+qtdLuz); p++, x++){
    if(EEPROM.read(p) >= 1){
      if(EEPROM.read(p) <= 50){
        tipoInterruptor[x] = EEPROM.read(p);                                // le variavel da eeprom
        Serial.printf("[EEPROM LOAD] Atualizando tipoInterruptor%d para %d\n",x, tipoInterruptor[x]);
        TelnetClient.printf("[EEPROM LOAD] Atualizando tipoInterruptor%d para %d\n",x, tipoInterruptor[x]);
      }
    }
  }



}
