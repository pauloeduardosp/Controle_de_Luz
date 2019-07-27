/* 
 * 
 * HTTP para reset do modulo
 *     
 */
 
void http_reset() {

  Serial.println("entrou no http reset");
  
  if (server.hasArg("cmd_post"))  {
    if(server.arg("cmd_post")=="reset") {
      Serial.println("reset esp ");
      for(int y=0; y < 3 ; y++){          // pisca a luz 3x para indicar o reset
        for(int p=0; p < qtdLuz; p++){
          digitalWrite(gpioPrimario[p],!digitalRead(gpioPrimario[p]));
          delay(500);
          digitalWrite(gpioPrimario[p],!digitalRead(gpioPrimario[p]));
          delay(500);
        }

        }
//      ESP.restart();
      ESP.reset();
//node.restart(); 
    }
  }                         




  String MEUIP = WiFi.localIP().toString();

  String buf = "";
     
  buf += "<!doctype html>\r\n";
  buf += "<html>\r\n";
  buf += "<head>\r\n";
//  buf += "<title>Reset "+nomeDevice+"</title>\r\n";
  buf += "<title>Reset "+mqttDeviceId+"</title>\r\n";
  buf += "<meta name=\"viewport\" content=\"width=320\">\r\n";
  buf += "<meta name=\"viewport\" content=\"width=device-width\">\r\n";
  buf += "<meta charset=\"utf-8\">\r\n";
  buf += "<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">\r\n";
  buf += "</head>\r\n";
  buf += "<body bgcolor=\"ADD8F6\">\r\n";
  buf += "<center>\r\n";
        
//  buf += "<h3> Reset "+nomeDevice+"</h3>\r\n";
  buf += "<h3> Reset "+mqttDeviceId+"</h3>\r\n";
  buf += "<TABLE BORDER=0 align='middle'>\r\n";
  buf += "<TD WIDTH=150 align='middle'><form action='/reset' method='post'>\r\n";
  buf += "<input type='hidden' name=cmd_post value=reset /></p>\r\n";
  buf += "<button type=submit style=\"width:105px;\" value=cmd_post>Modulo Reset</button>\r\n";
  buf += "</form> <br/></td>\r\n"; 


  buf += "</table>\r\n";  
  buf += "</center>\r\n";
        
  buf += "</body>\r\n";
  buf += "</html>\r\n";

  server.send(200, "text/html", buf);  



}
