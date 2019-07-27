void dhtsetup(){

  for(int p = 0; p < qtdLuz; p++){
    if(tipoDispositivo[p] == 2 || tipoDispositivo[p] == 3){
    
      uint8_t type;

      switch (tipoDispositivo[p]){
        case 2:      // DHT11
          type = DHT11; 
        break;
        
        case 3:      // DHT 22  (AM2302), AM2321
          type = DHT21;
        break;
      }
      
      // create the object as static.
      static DHT static_dht(gpioPrimario[p], type);
      // save its address.
      dht = &static_dht;
      // call begin() and your ready to use the sensor if correctly wired.
      dht->begin();
      
    }
  }
  
}

void dht_loop(){
  for(int p = 0; p < qtdLuz; p++){
    if(tipoDispositivo[p] == 2 || tipoDispositivo[p] == 3){
        
      float umidade;
      float temperatura;
      float s_umidade = 0;
      float s_temperatura = 0;  
      String value;
      float m_temperatura;
      float m_umidade;
      
      now = millis();
      
      if (qtdDht>= 30){
        qtdDht = 0;
      }
      
    
      if(now - ultimaMedicaoDht >= 3000) {
        ultimaMedicaoDht = millis();
        dht->read();
        umidade = dht->readHumidity();
        temperatura = dht->readTemperature();
        
        if (isnan(temperatura) || isnan(umidade)) {
          Serial.print(millis()/1000);
          Serial.println(" Failed to read from DHT");
        } else {
          v_umidade[qtdDht] = umidade;
          v_temperatura[qtdDht] = temperatura;
          qtdDht ++;
      
          for(int x=0; x < 30; x++){
            s_umidade += v_umidade[x];
            s_temperatura += v_temperatura[x];
          }
        
          m_umidade = s_umidade/30;
          m_temperatura = s_temperatura/30;
    
          if(now - ultimoEnvioDht >= 300000) {
            ultimoEnvioDht = millis();
            String payloadStatus;
            payloadStatus = "{\"";
            value = String(m_umidade);
            payloadStatus += "u\":\""+value+"\"";
            payloadStatus += ",";
            value = String(m_temperatura);
            payloadStatus += "\"t\":\""+value+"\"";
            payloadStatus += "}";
            
            String st_value = mqttAtuadorId[p];
            char* c_topicoPublish = &st_value[0u];
            char* c_payloadStatus = &payloadStatus[0u];
            client.publish(c_topicoPublish, c_payloadStatus, 1);
            Serial.print(millis()/1000);
            Serial.println(" Enviou status dht 5 min");
          }
        }
      }
    }
  }
}



