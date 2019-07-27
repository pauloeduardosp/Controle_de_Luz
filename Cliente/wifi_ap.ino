/*
 * 
 * para de divulgar ap apos 5 min caso wifi ja esteja conectado
 *
 */ 
void wifi_ap(){

//##########################################################
//#### função parar de divulgar AP
  if(WiFi.status() != WL_CONNECTED){
    divulgaAP = true;
  }
  
  if(divulgaAP){
    if(millis() > 300000){//300000){
      if (WiFi.status() == WL_CONNECTED){
        Serial.print("WiFi connected - ESP IP address: ");
        Serial.println(WiFi.localIP());
        WiFi.softAPdisconnect(true);
        divulgaAP = false;
        
        Serial.println("[Wifi_status] Parou de divulgar AP ");
      }
    }
  }

//#### fim função parar de divulgar AP
//##########################################################

}
