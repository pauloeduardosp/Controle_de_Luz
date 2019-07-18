/*
 * 
 *
 * Pagina HTTP de configuração
 *
 */
  
void http_config() {
  Serial.println(" [HTTP_C] Entrou na funcao http_config()");
  TelnetClient.println("[HTTP_C] Entrou na funcao http_config()");


//##########################################################################################
// dados de data
  int segundos =  (millis()/(1000) % 60);  
  int minutos =   (millis()/(60000) % 60);   // (millis()/(1000*60)%60
  int horas =     (millis()/(3600000) % 24);   // (millis()/(1000*60*60)%60  
  int dias =      (millis()/(86400000)  % 24);       // (milliseconds / (1000*60*60*24)) % 24);
  String strgD = String(dias);
  String strgH;
  String strgM;
  String strgS;
  
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
  if(segundos <10){
    strgS = "0"+String(segundos);
  } else {
    strgS = String(segundos);
  }
  
// fim dados de data
//##########################################################################################

  
  String value;
  String valuep;

  String buf = "";
// Cabeçalho http padrão
  buf += "<!doctype html>\r\n";
  buf += "<html>\r\n";

  buf += "<head>\r\n";
//  buf += "<head><meta http-equiv=\"refresh\" content=\"1;URL=http://pauloeduardosp.no-ip.org:82\"></head>\n";

  buf += "<title>Domotica Config Cliente</title>\r\n";
  buf += "<meta name=\"viewport\" content=\"width=320\">\r\n";
  buf += "<meta name=\"viewport\" content=\"width=device-width\">\r\n";
  buf += "<meta charset=\"utf-8\">\r\n";
  buf += "<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">\r\n";

// ####################################################
// Style 
  buf += "<style>\r\n";
  buf += "button{\r\n";
  buf += "      cursor:pointer;\r\n";
  buf += "      border:0;border-radius:0.5rem;\r\n";
  buf += "      line-height:1.3rem;\r\n";
  buf += "      font-size:0.8rem;\r\n";
  buf += "      width:4.0rem;\r\n";
  buf += "      font-weight: bold;\r\n";
  buf += "    }\r\n";
  
  buf += "  input[type=\"number\"] {\r\n";
  buf += "   width:30px;\r\n";
  buf += "}\r\n";
  buf += "</style>\r\n";
// ####################################################

  buf += "</head>\r\n";
  buf += "<body bgcolor=\"ADD8E6\">\r\n";
  buf += "<font face=\"arial\" size=\"3\">\r\n";
  buf += "<center>\r\n";
  buf += "<BR>\r\n";
  buf += "<form method=POST>\r\n";

  buf += "<title>Domotica Config Cliente</title>\r\n";
  buf += "<h3> Configuração Cliente</h3>\r\n";

// ####################################################
// Status da rede wifi
  buf += "<table>\r\n";

  if(WiFi.status() == WL_CONNECTED){
    buf += "<tr>\r\n";
    buf += "<td>Status:</td>\r\n";
    buf += "<td><b>Conectado</b></td>\r\n";
    buf += "</tr>\r\n";

    value = String(WiFi.SSID());
    buf += "<tr>\r\n";
    buf += "<td width=100px>Rede Wifi:</td>\r\n";
    buf += "<td>"+value+"</td>\r\n";
    buf += "</tr>\r\n";

    value = WiFi.localIP().toString();
    buf += "<tr>\r\n";
    buf += "<td>IP:</td>\r\n";
    buf += "<td>"+value+"</td>\r\n";
    buf += "</tr>\r\n";
    buf += "</table>\r\n";
  } else {
    buf += "<tr>\r\n";
    buf += "<td>Status:</td>\r\n";
    buf += "<td><b>Desconectado</b></td>\r\n";
    buf += "</tr>\r\n";
    
    value = String(wifi.ssid);
    buf += "<tr>\r\n";
    buf += "</tr>\r\n";
    buf += "</table>\r\n";
    buf += "<label>Tentando conectar a rede <b>"+value+"</b>\r\n";
  }


// ####################################################
// Config Cliente

  buf += "<table>\r\n";
  buf += "<BR>\r\n";
  buf += "<tr>\r\n";
  buf += "<td width = 115px><label>SSID: </label></td>\r\n";
  value = String(wifi.ssid);
  buf += "<td><input name=POST_wifi_ssid  type=text value=\""+value+"\" size=\"15\"></td>\r\n";
  buf += "</tr>\r\n";


  buf += "<tr>\r\n";
  buf += "<td><label>Senha: </label></td>\r\n";
  value = String(wifi.password);
  buf += "<td><input type=\"password\" name=POST_wifi_password type=text  value=\""+value+"\" size=15></td>\r\n";
  buf += "</tr>\r\n";

  buf += "<tr>\r\n";
  buf += "<td><label>IP do Server:</label></td>\r\n";



  if(octeto[0] != 0){
    value = String(octeto[0]);
    value += ".";
    value += String(octeto[1]);
    value += ".";
    value += String(octeto[2]);
    value += ".";
    value += String(octeto[3]);
  } else {
    value = "";
  }


//  buf += "<td><input name=POST_ipServer type=text value=\""+value+"\" size=\"15\"></td>\r\n";
  buf += "</tr>\r\n";

  buf += "<tr>\r\n";
  buf += "<td><label>Nome Device:</label></td>\r\n";
  value = String(nomeDevice);
  buf += "<td><input name=POST_nomeDevice type=text value=\""+value+"\" size=15></td>\r\n";
  buf += "</tr>\r\n";

  buf += "<tr>\r\n";
  buf += "<td width = 115px><label>Qtd de Luzes:</label></td>\r\n";
  buf += "<td>\r\n";


  buf += "<select name=POST_qtdLuz>\r\n";
  for(int p = 1; p <= 4; p++){
  value = String(p);
    if(p==qtdLuz){
      buf += "<option value=\""+value+"\" selected>"+value+"</option>\r\n";
    }else{
      buf += "<option value=\""+value+"\">"+value+"</option>\r\n";
    }
  }
  buf += "</select>\r\n";
  buf += "</td>\r\n";
  buf += "</table>\r\n";

  IPAddress ip = WiFi.localIP();
  value = String(ip[3]);
  buf += "<input TYPE=\"hidden\" name=POST_idClient value="+value+">\r\n";

  buf += "<BR>\r\n";
  buf += "<hr align=\"center\" width=\"380\" size=\"3\" color=black>\r\n";

  // ####################################################
  // Configuração Luz

  buf += "<h3> Configuração Luz</h3>\r\n";
  buf += "<table>\r\n";

  buf += "<tr aligh = left>\r\n";
  buf += "<td width=\"50px\"><label> &nbsp </label></td>\r\n";
  buf += "<td width=\"50px\"><label> ID: </label></td>\r\n";
  buf += "<td width=\"50px\"><label> GPIO:</label></td>\r\n";
  buf += "<td width=\"100px\"><label> Nome: </label></td>\r\n";
  buf += "<td><label> IDServer: </label></td>\r\n";
  buf += "</tr>\r\n";

  for(int p = 0; p < qtdLuz; p++){
    buf += "<tr align=left>\r\n";
    valuep = String(p);
    buf += "<td><label> Luz "+valuep+" </label></td>\r\n";
    value = String(idLuz[p]);
    buf += "<td><input type=\"number\" name=POST_idLuz"+valuep+" type=text value="+value+"></td>\r\n";
    value = String(gpioLuz[p]);
    buf += "<td><input type=\"number\" name=POST_gpioLuz"+valuep+" type=text value="+value+"></td>\r\n";
    value = String(nomeLuz[p]);
    buf += "<td><input name=POST_nomeLuz"+valuep+" type=text value=\""+value+"\" size=\"10\"></td>\r\n";

    value = String(idServer[p]);
    buf += "<td align = center><input TYPE=\"hidden\" name=POST_idServer value="+value+">"+value+"</td>\r\n";

        buf += "<form method=POST>\r\n";
        buf += "<input TYPE=\"hidden\" name=POST_clearIdServer value="+valuep+">\r\n";
        buf += "<td><button type=\"submit\">Clear</button></form></td>\r\n";
        buf += "</tr>\r\n";
    
  }

  buf += "</tr>\r\n";

  buf += "</table>\r\n";

  buf += "<BR>\r\n";
  buf += "<hr align=\"center\" width=\"380\" size=\"3\" color=black>\r\n";

  // ####################################################
  // Configuração Interruptor

  buf += "<h3>Configuração Interruptor</h3>\r\n";
  buf += "<table>\r\n";

  buf += "<tr align = left>\r\n";
  buf += "<td width=\"100px\"><label>&nbsp&nbsp&nbsp&nbsp&nbsp </label></td>\r\n";
  buf += "<td width=\"120px\"><label>Tipo:</label></td>\r\n";
  buf += "<td width=\"100px\"><label>GPIO:</label></td>\r\n";
  buf += "</tr>\r\n";

  for(int p = 0; p < qtdLuz; p++){
    buf += "<tr>\r\n";
    valuep = String(p);
    buf += "<td><label> Interruptor "+valuep+"</label></td>\r\n";
    buf += "<td>\r\n";
    buf += "<select name=\"POST_tipoInterruptor"+valuep+"\">\r\n";
    if(tipoInterruptor[p] == 1){
      buf += "<option value=\"1\" selected>Paralelo</option>\r\n";
    } else {
      buf += "<option value=\"1\">Paralelo</option>\r\n";
    }

    if(tipoInterruptor[p] == 2){
      buf += "<option value=\"2\" selected>ligado no Gpio</option>\r\n";
    } else {
      buf += "<option value=\"2\">ligado no Gpio</option>\r\n";
    }

    buf += "</select>\r\n";
    buf += "</td>\r\n";


    value = String(gpioInterruptor[p]);
    buf += "<td><input type=\"number\"  name=POST_gpioInterruptor"+valuep+" type=text value="+value+"></td>\r\n";

    buf += "</tr>\r\n";
  }

  buf += "</table>\r\n";

  buf += "<BR><BR>\r\n";
  buf += "<button type=\"submit\">Salvar</button></form>\r\n";
  buf += "<BR><BR>\r\n";
  buf += "Up-Time "+strgD+"d "+strgH+":"+strgM+":"+strgS+"";
  buf += "</center>\r\n";
  buf += "</body>\r\n";
  buf += "</html>\r\n";

  server.send(200, "text/html", buf);






}

//################################################################################################
//################################################################################################


void http_config_post() {
  Serial.println("[HTTP_C] Entrou na funcao http_config_post()");
  TelnetClient.println("[HTTP_C] Entrou na funcao http_config_post()");

  String value;
  String valuep;
  String MEUIP = WiFi.localIP().toString();
  String buf = "";
  buf += "<!doctype html>\r\n";
  buf += "<html>\r\n";
  buf += "<head>\r\n";

  int valueInt = server.arg("POST_clearIdServer").toInt();
  if(valueInt <= qtdLuz){
    Serial.printf("[HTTP_C] Executaru clear do idserver \n");
    
    idServer[valueInt] = 0;
     
  }

    if(server.arg("POST_qtdLuz").toInt() != qtdLuz){
      
      Serial.println("Qtd Luz diferente");
      qtdLuz = server.arg("POST_qtdLuz").toInt();
      buf += "<head><meta http-equiv=\"refresh\" content=\"0;URL=http://"+MEUIP+":82/config\"></head>\n";
      buf += "</head>\r\n";
      buf += "<body bgcolor=\"ADD8E6\">\r\n";
      buf += "<center>\r\n";
      buf += "<h3>Atualizando quantidade de Luz<BR></h3>\r\n";
      buf += "</center>\r\n";
      buf += "</body>\r\n";
      buf += "</html>\r\n";
      server.send(200, "text/html", buf);
  
    } else {
      
      buf += "<head><meta http-equiv=\"refresh\" content=\"8;URL=http://"+MEUIP+":82/config\"></head>\n";
    
      buf += "<title>Domotica Config Cliente</title>\r\n";
      buf += "<meta name=\"viewport\" content=\"width=320\">\r\n";
      buf += "<meta name=\"viewport\" content=\"width=device-width\">\r\n";
      buf += "<meta charset=\"utf-8\">\r\n";
      buf += "<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">\r\n";
      buf += "</head>\r\n";
    
      buf += "<body bgcolor=\"ADD8E6\">\r\n";
      buf += "<center>\r\n";
    
      buf += "<h3> Config Cliente Web Server Luz</h3>\r\n";
      buf += "<p>Dados salvo, realizando reset no modulo<BR></p>\r\n";
    
    //########################
    // coleta os dados vindo do post http

      if(server.arg("POST_wifi_ssid") != ""){
        server.arg("POST_wifi_ssid").toCharArray(wifi.ssid, 30);
      } 
   
      if(server.arg("POST_wifi_password") != ""){
        server.arg("POST_wifi_password").toCharArray(wifi.password, 20);  
      } 

//      if(server.arg("POST_ipServer") != "" || server.arg("POST_ipServer") != 0){
//
//        char CHAR_ipServer[15];
//
//        String(server.arg("POST_ipServer")).toCharArray(CHAR_ipServer,16);
//        String STRING_octeto;
//        int y = 0;
//        for(int p = 0; p <= 16; p++){
//          if(CHAR_ipServer[p] != '.' &&  CHAR_ipServer[p] >= '0' && CHAR_ipServer[p] <= '9' ){
//              STRING_octeto +=  CHAR_ipServer[p];
//          } else {
//              octeto[y] = STRING_octeto.toInt();
//              STRING_octeto = "";
//              y++;
//          }
//        }
//      }         
    


      if(server.arg("POST_qtdLuz").toInt() != 0){
        if(qtdLuz !=server.arg("POST_qtdLuz").toInt()){
          qtdLuz = server.arg("POST_qtdLuz").toInt();
        }
      }
    
      if(server.arg("POST_nomeDevice") != ""){
        nomeDevice = String(server.arg("POST_nomeDevice")) ;
      }

      for(int p = 0; p < qtdLuz; p++){
        String POST_idLuz = "POST_idLuz";
        POST_idLuz += String(p);
        if(server.arg(""+POST_idLuz+"") != ""){
          idLuz[p]= server.arg(""+POST_idLuz+"").toInt();
        }
    
        String POST_gpioLuz = "POST_gpioLuz";
        POST_gpioLuz += String(p);
        if(server.arg(""+POST_gpioLuz+"") != ""){
          gpioLuz[p]= server.arg(""+POST_gpioLuz+"").toInt();
        }
    
        String POST_nomeLuz = "POST_nomeLuz";
        POST_nomeLuz += String(p);
        if(server.arg(""+POST_nomeLuz+"") != ""){
          nomeLuz[p]= String(server.arg(""+POST_nomeLuz+""));
        }
    
        String POST_gpioInterruptor = "POST_gpioInterruptor";
        POST_gpioInterruptor += String(p);
        if(server.arg(""+POST_gpioInterruptor+"") != ""){
          gpioInterruptor[p]= server.arg(""+POST_gpioInterruptor+"").toInt();
        }
    
        String POST_tipoInterruptor = "POST_tipoInterruptor";
        POST_tipoInterruptor += String(p);
        if(server.arg(""+POST_tipoInterruptor+"") != ""){
          tipoInterruptor[p]= server.arg(""+POST_tipoInterruptor+"").toInt();
        }
    
      }
    
    
    
    
    //########################
    // ####################################################
    // Config Cliente
      buf += "<BR><BR><BR>\r\n";
      buf += "<h3> Configuração Cliente</h3>\r\n";
    
      buf += "<table>\r\n";
      
      buf += "<tr>\r\n";
      buf += "<td><label>Wifi: &nbsp&nbsp </label></td>\r\n";
      value = String(wifi.ssid);
      buf += "<td><label>"+value+"</label></td>\r\n";
      buf += "</tr>\r\n";
      
      buf += "<tr>\r\n";
      buf += "<td><label>Senha: &nbsp&nbsp </label></td>\r\n";
      value = String(wifi.password);
      buf += "<td><label>"+value+"</label></td>\r\n";
      buf += "</tr>\r\n";
      
      buf += "<tr>\r\n";
      buf += "<td><label>Nome Device: &nbsp&nbsp </label></td>\r\n";
      value = String(nomeDevice);
      buf += "<td><label>"+value+"</label></td>\r\n";
      buf += "</tr>\r\n";
    
      buf += "<tr>\r\n";
      buf += "<td><label>Quantidade de Luzes:</label></td>\r\n";
      value = String(qtdLuz);
      buf += "<td><label>"+value+"</label></td>\r\n";
      buf += "</tr>\r\n";
    
      buf += "<tr>\r\n";
      buf += "<td><label>Id Cliente:</label></td>\r\n";
      value = String(idClient);
      buf += "<td><label>"+value+"</label></td>\r\n";
      buf += "</tr>\r\n";
    
      buf += "</table>\r\n";
    
      buf += "<BR><BR>\r\n";
      buf += "<hr align=\"center\" width=\"430\" size=\"3\" color=black>\r\n";
    
      // ####################################################
      // Configuração Luz
    
      buf += "<h3> Configuração Luz</h3>\r\n";
      buf += "<table>\r\n";
      buf += "<tr>\r\n";
      buf += "<td width=\"50px\"><label> &nbsp </label></td>\r\n";
      buf += "<td width=\"50px\"><label> &nbsp ID: </label></td>\r\n";
      buf += "<td width=\"50px\"><label> &nbsp ID Server: </label></td>\r\n";
      buf += "<td width=\"80px\"><label> &nbsp&nbsp&nbsp GPIO:</label></td>\r\n";
      buf += "<td><label> Nome: </label></td>\r\n";
      buf += "<td><label> IdServer: </label></td>\r\n";
      buf += "</tr>\r\n";
    
    
      for(int p = 0; p < qtdLuz; p++){
        buf += "<tr align=center>\r\n";
        valuep = String(p);
        buf += "<td><label> Luz "+valuep+" </label></td>\r\n";
        value = String(idLuz[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";
        value = String(idServer[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";
        value = String(gpioLuz[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";
        value = String(nomeLuz[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";
        value = String(idServer[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";
      }
      buf += "</table>\r\n";
    
      buf += "<BR><BR>\r\n";
      buf += "<hr align=\"center\" width=\"430\" size=\"3\" color=black>\r\n";
    
      // ####################################################
      // Configuração Interruptor
    
      buf += "<h3>Configuração Interruptor</h3>\r\n";
      buf += "<table>\r\n";
    
      buf += "<tr>\r\n";
      buf += "<td width=\"120px\"><label>&nbsp&nbsp&nbsp&nbsp&nbsp </label></td>\r\n";
      buf += "<td width=\"150px\"><label>Tipo:</label></td>\r\n";
      buf += "<td width=\"100px\"><label>GPIO:</label></td>\r\n";
      buf += "</tr>\r\n";
    
    
      for(int p = 0; p < qtdLuz; p++){
        buf += "<tr>\r\n";
        valuep = String(p);
        buf += "<td><label> Interruptor "+valuep+"</label></td>\r\n";
    
        switch(tipoInterruptor[p]){
          case 1:
            value = "Paralelo";
            break;
    
          case 2:
            value = "Ligado ao GPIO";
            break;
        }
        buf += "<td><label>"+value+"</label></td>\r\n";
    
        value = String(gpioInterruptor[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";
    
        buf += "</tr>\r\n";
      }
      buf += "</table>\r\n";
    
    
      buf += "<BR><BR>\r\n";
      buf += "<hr align=\"center\" width=\"430\" size=\"3\" color=black>\r\n";
    
    
      buf += "</body>\r\n";
      buf += "</html>\r\n";
    
      server.send(200, "text/html", buf);
  

      saveConfig();
    

  }
}



