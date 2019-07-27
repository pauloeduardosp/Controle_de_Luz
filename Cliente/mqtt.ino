void mqtt_setup(){

//######################
//##  extrai o mqttDeviceId da variavel mqttAtuadorId

  int indexBuscaBarra;
  for(int p=0; p <= mqttAtuadorId[0].length(); p++){
    if(mqttAtuadorId[0][p]=='/'){
      indexBuscaBarra = p;
    }
  }
  
  mqttDeviceId = "                                                  ";
  for(int p=0; p < indexBuscaBarra; p++){
    mqttDeviceId[p]= mqttAtuadorId[0][p];
  }

  mqttDeviceId.trim();          // excluir espaço em branco na variavel
  mqttDeviceId.toLowerCase();   // deixar a variavel inteira em caixa baixa
  
//##   Fim
//######################

//######################
//  extrai o mqttPlaceId da variavel mqttDeviceId

   for(int p=0; p <= mqttDeviceId.length(); p++){
    if(mqttDeviceId[p]=='/'){
      indexBuscaBarra = p;
    }
  }
  
  mqttPlaceId = "                                                  ";
  for(int p=0; p < indexBuscaBarra; p++){
    mqttPlaceId[p]= mqttDeviceId[p];
  }

  mqttPlaceId.trim();          // excluir espaço em branco na variavel
  mqttPlaceId.toLowerCase();   // deixar a variavel inteira em caixa baixa
  
//   Fim
//######################

  char* C_mqttServer = &mqttServer[0u];

  client.setServer(C_mqttServer, 1883); 
  client.setCallback(callback);


}


void callback(String topic, byte* message, unsigned int length) {
  Serial.print("[MQTT Callback] Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

    for(int p = 0;p < qtdLuz; p++){
      
      String value = mqttAtuadorId[p];
      value += change;
      char* c_value = &value[0u];

      String valuep = mqttAtuadorId[p];
      char* c_valuep = &valuep[0u];


      if(topic==c_value){
//        verificacaoStatusLampada = true;  
        Serial.print("Changing Room lamp to ");
        if(messageTemp == "change"){
          digitalWrite(gpioPrimario[p],!digitalRead(gpioPrimario[p]));
          if(digitalRead(gpioPrimario[p])){
            Serial.println("On");  
          } else {
            Serial.println("off");  
          }
          
        }

//        if(messageTemp == "on"){
//          digitalWrite(gpioPrimario[p], HIGH);
//          Serial.println("On");
//        }
//        else if(messageTemp == "off"){
//          digitalWrite(gpioPrimario[p], LOW);
//          Serial.println("Off ");
//
//        }
      }
    }
  Serial.println();

}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
    now = millis();


  // Loop until we're reconnected
  if (WiFi.status() == WL_CONNECTED){
    if (now - lastMeasure > 3000) {  
      Serial.print(millis()/1000);
      Serial.print("[MQTT reconnect] Attempting MQTT connection...");
      // Attempt to connect
  
          String value;
          String payloadTestamento;
          payloadTestamento = "{\"";
          payloadTestamento += "ip\":\""+WiFi.localIP().toString()+"\"";
          payloadTestamento += ",";
          payloadTestamento += "\"device\":\""+mqttDeviceId;
          payloadTestamento += "\",";
          payloadTestamento += "\"versao\":\""+versao+"\"";
          payloadTestamento += ",";
          payloadTestamento += "\"status\":\"red\"";
          payloadTestamento += "}";
      
      char* c_payloadTestamento = &payloadTestamento[0u];
  
      char* c_mqttDeviceId =  &mqttDeviceId[0u];
  
        String  s_topicTestamento = mqttPlaceId;
                s_topicTestamento += "/status";
        char*   c_topicTestamento = &s_topicTestamento[0u];
  
        if (client.connect(c_mqttDeviceId, c_topicTestamento, 1, false, c_payloadTestamento)) {    
        
          Serial.println("connected");  
          // Subscribe or resubscribe to a topic
          // You can subscribe to more topics (to control more LEDs in this example)
          verificacaoStatusLampada = true;          // variavel para fazer update apos conexão
          mqttUpdate = true;
          Serial.print("############# mqttUpdate ");
          Serial.println(mqttUpdate);
          
          String value = mqttDeviceId;
          value += "/+";
          value += change;
          char* c_topicosubscribe = &value[0u];
          Serial.print("Fazendo subcribe em ");
          Serial.println(c_topicosubscribe);
          client.subscribe(c_topicosubscribe);
          lastMqttStatus = true;
        
        } else {
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 3 seconds");
          lastMeasure = millis();
          lastMqttStatus = false;
      }
    }
  }
}


void mqttEspStatus(){


//##########################################################################################
// dados de data
  int minutos =   (millis()/(60000) % 60);   // (millis()/(1000*60)%60
  int horas =     (millis()/(3600000) % 24);   // (millis()/(1000*60*60)%60  
  int dias =      (millis()/(86400000)  % 24);       // (milliseconds / (1000*60*60*24)) % 24);
  String strgD = String(dias);
  String strgH;
  String strgM;
  
  if(horas  < 10){
    strgH = "0"+String(horas);
  } else {
    strgH = String(horas);
  }
  if(minutos <10){
    strgM = "0"+String(minutos);
  } else {
    strgM = String(minutos);
  }
String upTime = strgD+"d "+strgH+":"+strgM;
  
// fim dados de data
//#######################################################################
  
  if(tempoUpdate < millis()){
    tempoUpdate = millis()+ 300000;
    mqttUpdate = true;    
  }


  if(mqttUpdate){
     char* c_mqttDeviceId =  &mqttDeviceId[0u];
     mqttUpdate = false;
     String value;


    String value1 = mqttPlaceId;
    value1 += "/status";
    char* c_topicoPublishSstatus = &value1[0u];

//###################################################
// Teste variavel mqtt json

        String payloadStatus;
        payloadStatus = "{\"";
        payloadStatus += "ip\":\""+WiFi.localIP().toString()+"\"";
        payloadStatus += ",";
        payloadStatus += "\"device\":\""+mqttDeviceId;
        payloadStatus += "\",";
        payloadStatus += "\"up\":\""+upTime+"\"";
        payloadStatus += ",";
        value = String(EEPROM.read(EEPROM_totalReset));
        payloadStatus += "\"reset\":\""+value+"\"";
        payloadStatus += ",";
        payloadStatus += "\"ver\":\""+versao+"\"";
        payloadStatus += ",";
        payloadStatus += "\"status\":\"green\"";
        payloadStatus += "}";

char* c_payloadStatus = &payloadStatus[0u];
//   FINAL     
//###################################################


    client.publish(c_topicoPublishSstatus, c_payloadStatus, 1);
  }

}
