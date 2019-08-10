/*
 * 
 *
 * tste ajax script antigo
 *
 */

  
void http_config() {
Serial.println(" [HTTP_C] Entrou na funcao http_config()");

//###########################################
teste_funcao();
//###########################################

  String value;
  String valuep;


  if(server.arg("POST_Update")=="qtdDispositivo"){
       qtdLuz = server.arg("Post_qtdDispositivo").toInt();
  }




  for(int p=0; p <= qtdLuz; p++){
    valuep = p;
    value = "tipoDispositivo"+valuep;

    if(server.arg("POST_Update")== value){
      tipoDispositivo[p] = server.arg("Post_tipoDispositivo"+valuep+"").toInt();
    }
  }    



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

  


  String buf = "";
// Cabeçalho http padrão
  buf += "<!doctype html>\r\n";
  buf += "<html>\r\n";

  buf += "<head>\r\n";
//  buf += "<head><meta http-equiv=\"refresh\" content=\"1;URL=http://pauloeduardosp.no-ip.org:82\"></head>\n";
  value = mqttDeviceId;
  buf += "<title>Domotica Config "+value+"</title>\r\n";
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
  buf += "<body bgcolor=\"ADD8F6\" id=\"demo\">\r\n";
  buf += "<font face=\"arial\" size=\"3\">\r\n";
  buf += "<center>\r\n";
  buf += "<BR>\r\n";
  buf += "<form action=\"configpost\" method=POST>\r\n";

  value = mqttDeviceId;
  buf += "<title>Domotica Config "+value+"</title>\r\n";
 // buf += "<title>Domotica Config Cliente</title>\r\n";
  buf += "<h3> Configuração Cliente</h3>\r\n";

// ####################################################
// Status da rede wifi
  buf += "<table>\r\n";

  if(WiFi.status() == WL_CONNECTED){
    buf += "  <tr>\r\n";
    buf += "    <td>Status:</td>\r\n";
    buf += "    <td><b>Conectado</b></td>\r\n";
    buf += "  </tr>\r\n";

    value = String(WiFi.SSID());
    buf += "  <tr>\r\n";
    buf += "   <td width=100px>Rede Wifi:</td>\r\n";
    buf += "   <td>"+value+"</td>\r\n";
    buf += "  </tr>\r\n";

    value = WiFi.localIP().toString();
    buf += "  <tr>\r\n";
    buf += "    <td>IP:</td>\r\n";
    buf += "    <td>"+value+"</td>\r\n";
    buf += "  </tr>\r\n";
    buf += "</table>\r\n";
  } else {
    buf += "  <tr>\r\n";
    buf += "    <td>Status:</td>\r\n";
    buf += "    <td><b>Desconectado</b></td>\r\n";
    buf += "  </tr>\r\n";
    
    value = String(wifi.ssid);
    buf += "  <tr>\r\n";
    buf += "  </tr>\r\n";
    buf += "</table>\r\n";
    buf += "<label>Tentando conectar a rede <b>"+value+"</b>\r\n";
  }


// ####################################################
// Config Cliente

  buf += "<table>\r\n";
  buf += "  <BR>\r\n";
  buf += "  <tr>\r\n";
  buf += "    <td width = 125px><label>Wifi SSID: </label></td>\r\n";
  value = String(wifi.ssid);
  buf += "    <td><input name=POST_wifi_ssid  type=text value=\""+value+"\" size=\"20\"></td>\r\n";
  buf += "  </tr>\r\n";


  buf += "  <tr>\r\n";
  buf += "    <td><label>Wifi Senha: </label></td>\r\n";
  value = String(wifi.password);
  buf += "    <td><input type=\"password\" name=POST_wifi_password type=text  value=\""+value+"\" size=20></td>\r\n";
  buf += "  </tr>\r\n";

  buf += "  <tr>\r\n";
  buf += "    <td><label>MQTT Server:</label></td>\r\n";
  value = String(mqttServer); 
  buf += "    <td><input name=POST_mqttServer type=text value=\""+value+"\" size=20></td>\r\n";
  buf += "  </tr>\r\n";

  buf += "    <td width = 125px><label>Mqtt User: </label></td>\r\n";
  value = String(mqttUser);
  buf += "    <td><input name=POST_mqttUser  type=text value=\""+value+"\" size=\"20\"></td>\r\n";
  buf += "  </tr>\r\n";

  buf += "  <tr>\r\n";
  buf += "    <td><label>Mqtt Senha: </label></td>\r\n";
  value = String(mqttPass);
  buf += "    <td><input type=\"password\" name=POST_mqttPass type=\"password\" value=\""+value+"\" size=20></td>\r\n";
  buf += "  </tr>\r\n";

  //############################################
  // script
          
    buf += "<script>\r\n";
    buf += "function loadQtdDispositivo() {\r\n";
    buf += "  var xhttp = new XMLHttpRequest();\r\n";
    buf += "  xhttp.onreadystatechange = function() {\r\n";
    buf += "    if (this.readyState == 4 && this.status == 200) {\r\n";
    buf += "      document.getElementById(\"demo\").innerHTML = this.responseText;\r\n";
    buf += "    }\r\n";
    buf += "  };\r\n";
    buf += "  xhttp.open(\"POST\", \"\", true);\r\n";
    buf += "  xhttp.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");\r\n";
    buf += "    var selectedValue = IDqtdDispositivo.value;\r\n";
    buf += "  xhttp.send(\"POST_Update=qtdDispositivo&Post_qtdDispositivo=\"+selectedValue+\"\");\r\n";  // funciona
    buf += "}\r\n";

  for(int p = 0; p <= 3; p++){ 
    valuep = p;

    buf += "function tipoDispositivo"+valuep+"() {\r\n";
    buf += "  var xhttp = new XMLHttpRequest();\r\n";
    buf += "  xhttp.onreadystatechange = function() {\r\n";
    buf += "    if (this.readyState == 4 && this.status == 200) {\r\n";
    buf += "      document.getElementById(\"demo\").innerHTML = this.responseText;\r\n";
    buf += "    }\r\n";
    buf += "  };\r\n";
    buf += "  xhttp.open(\"POST\", \"\", true);\r\n";
    buf += "  xhttp.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");\r\n";
    buf += "    var selectedValue"+valuep+" = IDtipoDispositivo"+valuep+".value;\r\n";
    buf += "  xhttp.send(\"POST_Update=tipoDispositivo"+valuep+"&Post_tipoDispositivo"+valuep+"=\"+selectedValue"+valuep+"+\"\");\r\n";  // funciona
    buf += "}\r\n";
  }    
    buf += "</script>\r\n";

// fim script
//####################################





  buf += "  <tr>\r\n";
  buf += "    <td width = 115px><label>Qtd de Dipositivo:</label></td>\r\n";
  buf += "    <td>\r\n";

  buf += "    <select id=\"IDqtdDispositivo\" onchange=\"loadQtdDispositivo()\"  name=POST_qtdLuz>\r\n";

  for(int p = 1; p <= 4; p++){
  valuep = String(p);
    if(p==qtdLuz){
      buf += "    <option value=\""+valuep+"\" selected>"+valuep+"</option>\r\n";
    }else{
      buf += "    <option value=\""+valuep+"\">"+valuep+"</option>\r\n";
    }
  }
  buf += "    </select>\r\n";
  buf += "  </td>\r\n";
  buf += "</table>\r\n";


  buf += "<BR>\r\n";
  buf += "<hr align=\"center\" width=\"380\" size=\"3\" color=black>\r\n";

  // ####################################################
  // Configuração Luz

  buf += "<h3> Configuração Dispositivo</h3>\r\n";
 
  buf += "<table>\r\n";

  buf += "  <tr aligh = left>\r\n";
  buf += "    <td width=\"50px\"><label> &nbsp </label></td>\r\n";
  buf += "    <td width=\"50px\"><label> GPIO:</label></td>\r\n";
  buf += "    <td width=\"50px\"><label> Interruptor: </label></td>\r\n"; 
  buf += "    <td width=\"50px\"><label> GPIO:</label></td>\r\n"; 
  buf += "    <td width=\"50px\"><label> MQTT: </label></td>\r\n"; 
  buf += "  </tr>\r\n";


  for(int p = 0; p < qtdLuz; p++){
    buf += "  <tr align=left>\r\n";
    valuep = String(p);
    switch (tipoDispositivo[p]){
      case 0:      // nova entrada
        buf += "  <td>\r\n"; 
        buf += "    <select id=\"IDtipoDispositivo"+valuep+"\" onchange=\"tipoDispositivo"+valuep+"()\"  name=POST_tipoDispositivo"+valuep+">\r\n";
        
        buf += "    <option value=\"0\" selected>Definir</option>\r\n";
        buf += "    <option value=\"1\">Luz</option>\r\n";
        buf += "    <option value=\"2\">DTH11</option>\r\n";
        buf += "    <option value=\"3\">DTH21</option>\r\n";
        buf += "    </select>\r\n";
        buf += "  </td>\r\n";

        buf += "  <td align = left> <label> ----- </label> </td>  \r\n";
        buf += "  <td align = center> <label> ----- </label> </td>\r\n";
        buf += "  <td align = left> <label> ----- </label> </td>  \r\n";
        buf += "  <td align = left> <label> ----- </label> </td>  \r\n";
        
      break;
      
      case 1:      // luz
        buf += "  <td>\r\n";
        buf += "    <select id=\"IDtipoDispositivo"+valuep+"\" onchange=\"tipoDispositivo"+valuep+"()\"  name=POST_tipoDispositivo"+valuep+">\r\n";
        buf += "    <option value=\"0\">Definir</option>\r\n";
        buf += "    <option value=\"1\" selected>Luz</option>\r\n";
        buf += "    <option value=\"2\">DHT11</option>\r\n";
        buf += "    <option value=\"3\">DHT21</option>\r\n";
        buf += "    </select>\r\n";
        buf += "  </td>\r\n";

        value = String(gpioPrimario[p]);
        buf += "    <td><input type=\"number\" name=POST_gpioPrimario"+valuep+" type=text value="+value+"></td>\r\n";
        buf += "    <td>\r\n";
       
        buf += "    <select name=\"POST_tipoInterruptor"+valuep+"\">\r\n";
          if(tipoInterruptor[p] == 1){
            buf += "      <option value=\"1\" selected>Paralelo</option>\r\n";
          } else {
            buf += "      <option value=\"1\">Paralelo</option>\r\n";
          }
      
          if(tipoInterruptor[p] == 2){
            buf += "      <option value=\"2\" selected>ligado no Gpio</option>\r\n";
          } else {
            buf += "      <option value=\"2\">ligado no Gpio</option>\r\n";
          }
        buf += "      </select>\r\n";
        buf += "    </td>\r\n";

        value = String(gpioSecundario[p]);
        buf += "    <td><input type=\"number\"  name=POST_gpioSecundario"+valuep+" type=text value="+value+"></td>\r\n";
        
        value = String(mqttAtuadorId[p]);
        buf += "    <td><input name=POST_mqttAtuadorId"+valuep+" type=text value=\""+value+"\" size=\""+value+"\"></td>\r\n";

              
      break;

      case 2:      // DHT11
        buf += "  <td>\r\n";
        buf += "    <select id=\"IDtipoDispositivo"+valuep+"\" onchange=\"tipoDispositivo"+valuep+"()\"  name=POST_tipoDispositivo"+valuep+">\r\n";
        buf += "    <option value=\"0\">Definir</option>\r\n";
        buf += "    <option value=\"1\">Luz</option>\r\n";
        buf += "    <option value=\"2\" selected>DHT11</option>\r\n";
        buf += "    <option value=\"3\">DHT21</option>\r\n";
        buf += "    </select>\r\n";
        buf += "  </td>\r\n";      
      
        value = String(gpioPrimario[p]);
        buf += "    <td><input type=\"number\" name=POST_gpioPrimario"+valuep+" type=text value="+value+"></td>\r\n";

        buf += "    <td align = center name=\"POST_tipoInterruptor"+valuep+"\" value=\"0\"> <label> ----- </label> </td>\r\n";
        buf += "    <td align = left> <label> ----- </label> </td>\r\n";

        value = String(mqttAtuadorId[p]);
        buf += "    <td><input name=POST_mqttAtuadorId"+valuep+" type=text value=\""+value+"\" size=\""+value+"\"></td>\r\n";

      break;

      case 3:      // DHT21
        buf += "  <td>\r\n";
        buf += "    <select id=\"IDtipoDispositivo"+valuep+"\" onchange=\"tipoDispositivo"+valuep+"()\"  name=POST_tipoDispositivo"+valuep+">\r\n";
        buf += "    <option value=\"0\">Definir</option>\r\n";
        buf += "    <option value=\"1\">Luz</option>\r\n";
        buf += "    <option value=\"2\">DHT11</option>\r\n";
        buf += "    <option value=\"3\" selected>DHT21</option>\r\n";
        buf += "    </select>\r\n";
        buf += "  </td>\r\n";      
      
        value = String(gpioPrimario[p]);
        buf += "    <td><input type=\"number\" name=POST_gpioPrimario"+valuep+" type=text value="+value+"></td>\r\n";

        buf += "    <td align = center name=\"POST_tipoInterruptor"+valuep+"\" value=\"0\"> <label> ----- </label> </td>\r\n";
        buf += "    <td align = left> <label> ----- </label> </td>\r\n";

        value = String(mqttAtuadorId[p]);
        buf += "    <td><input name=POST_mqttAtuadorId"+valuep+" type=text value=\""+value+"\" size=\""+value+"\"></td>\r\n";

      break;
    }
 
  }

  buf += "  </tr>\r\n";

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



void http_config_post() {
  Serial.println("[HTTP_C] Entrou na funcao http_config_post()");




  String value;
  String valuep;
  String MEUIP = WiFi.localIP().toString();
  String buf = "";
  buf += "<!doctype html>\r\n";
  buf += "<html>\r\n";
  buf += "<head>\r\n";
  buf += "<head><meta http-equiv=\"refresh\" content=\"5;URL=http://"+MEUIP+":82/config\"></head>\n";

  buf += "<title>Domotica Config Cliente</title>\r\n";
  buf += "<meta name=\"viewport\" content=\"width=320\">\r\n";
  buf += "<meta name=\"viewport\" content=\"width=device-width\">\r\n";
  buf += "<meta charset=\"utf-8\">\r\n";
  buf += "<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">\r\n";
  buf += "</head>\r\n";

  buf += "<body bgcolor=\"ADD8F6\">\r\n";
  buf += "<center>\r\n";

  value = mqttDeviceId;
  buf += "<h3> Config Device "+value+" </h3>\r\n";
  buf += "<p>Dados salvo, realizando reset no modulo<BR></p>\r\n";

//########################
// coleta os dados vindo do post http

  if(server.arg("POST_wifi_ssid") != ""){
    server.arg("POST_wifi_ssid").toCharArray(wifi.ssid, 30);
  } 

  if(server.arg("POST_wifi_password") != ""){
    server.arg("POST_wifi_password").toCharArray(wifi.password, 20);  
  } 


  if(server.arg("POST_qtdLuz").toInt() != 0){
    if(qtdLuz !=server.arg("POST_qtdLuz").toInt()){
      qtdLuz = server.arg("POST_qtdLuz").toInt();
    }
  }


  if(server.arg("POST_mqttServer") != ""){
    mqttServer = String(server.arg("POST_mqttServer")) ;
  }

  if(server.arg("POST_mqttUser") != ""){
    mqttUser = String(server.arg("POST_mqttUser")) ;
  } 

  if(server.arg("POST_mqttPass") != ""){
    mqttPass = String(server.arg("POST_mqttPass")) ;
  } 

  for(int p = 0; p < qtdLuz; p++){
    String POST_gpioPrimario = "POST_gpioPrimario";
    POST_gpioPrimario += String(p);
    if(server.arg(""+POST_gpioPrimario+"") != ""){
      gpioPrimario[p]= server.arg(""+POST_gpioPrimario+"").toInt();
    }

    String POST_mqttAtuadorId = "POST_mqttAtuadorId";
    POST_mqttAtuadorId += String(p);
    if(server.arg(""+POST_mqttAtuadorId+"") != ""){
      mqttAtuadorId[p]= String(server.arg(""+POST_mqttAtuadorId+""));
    }

    
    String POST_gpioSecundario = "POST_gpioSecundario";
    POST_gpioSecundario += String(p);
    if(server.arg(""+POST_gpioSecundario+"") != ""){
      gpioSecundario[p]= server.arg(""+POST_gpioSecundario+"").toInt();
    }
    
    String POST_tipoInterruptor = "POST_tipoInterruptor";
    POST_tipoInterruptor += String(p);
    if(server.arg(""+POST_tipoInterruptor+"") != ""){
      tipoInterruptor[p]= server.arg(""+POST_tipoInterruptor+"").toInt();
    }

    String POST_tipoDispositivo = "POST_tipoDispositivo";
    POST_tipoDispositivo += String(p);
    if(server.arg(""+POST_tipoDispositivo+"") != ""){
      tipoDispositivo[p]= server.arg(""+POST_tipoDispositivo+"").toInt();
    }
    Serial.printf("POST_tipoDispositivo%d ",p);
    Serial.println(server.arg(""+POST_tipoDispositivo+""));
    
  }
    
  
//########################
// ####################################################
// Config Cliente
  buf += "<BR><BR><BR>\r\n";
  buf += "<h3> Configuração Device</h3>\r\n";

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
  buf += "<td><label>MQTT Server: &nbsp&nbsp </label></td>\r\n";
  value = String(mqttServer);
  buf += "<td><label>"+value+"</label></td>\r\n";
  buf += "</tr>\r\n";

  buf += "<tr>\r\n";
  buf += "<td><label>MQTT User: &nbsp&nbsp </label></td>\r\n";
  value = String(mqttUser);
  buf += "<td><label>"+value+"</label></td>\r\n";
  buf += "</tr>\r\n";

  buf += "<tr>\r\n";
  buf += "<td><label>MQTT Senha: &nbsp&nbsp </label></td>\r\n";
  value = String(mqttPass);
  buf += "<td><label>"+value+"</label></td>\r\n";
  buf += "</tr>\r\n";

  buf += "<tr>\r\n";
  buf += "<td><label>Quantidade de Device:</label></td>\r\n";
  value = String(qtdLuz);
  buf += "<td><label>"+value+"</label></td>\r\n";
  buf += "</tr>\r\n";


  buf += "</table>\r\n";

  buf += "<BR><BR>\r\n";
  buf += "<hr align=\"center\" width=\"430\" size=\"3\" color=black>\r\n";

  // ####################################################
  // Configuração Luz



  buf += "<h3> Configuração Dispositivo</h3>\r\n";
  buf += "  <table>\r\n";
  buf += "    <tr>\r\n";
  buf += "      <td width=\"50px\"><label> &nbsp </label></td>\r\n";
  buf += "      <td width=\"80px\"><label> &nbsp&nbsp&nbsp GPIO:</label></td>\r\n";
  buf += "      <td width=\"100px\"><label> &nbsp&nbsp Interruptor</label></td>\r\n";
  buf += "      <td width=\"80px\"><label> &nbsp&nbsp&nbsp GPIO:</label></td>\r\n";
  buf += "      <td width=\"80px\"><label> &nbsp&nbsp&nbsp MQTT:</label></td>\r\n";

  buf += "</tr>\r\n";



  for(int p = 0; p < qtdLuz; p++){
    valuep = String(p);
    buf += "<tr align=center>\r\n";

    switch(tipoDispositivo[p]){
      case 1:
        value = "Luz";

        buf += "<td><label> "+value+" </label></td>\r\n";
      
        value = String(gpioPrimario[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";
        
        if(tipoInterruptor[p]=1){
          value = "Paralelo";
        } else if(tipoInterruptor[p]){
          value = "Ligado ao GIPIO";
        }
        
        buf += "<td><label>"+value+"</label></td>\r\n";
    
        value = String(gpioSecundario[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";
    
        value = String(mqttAtuadorId[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";


        
        break;

      case 2:
        value = "DHT";
        buf += "<td><label> "+value+" </label></td>\r\n";
      
        value = String(gpioPrimario[p]);
        buf += "<td><label>"+value+"</label></td>\r\n";
        buf += "    <td align = center> <label> ----- </label> </td>\r\n";
        buf += "    <td align = left> <label> ----- </label> </td>\r\n";
        break;
    }

  }
  buf += "</table>\r\n";

  buf += "<BR><BR>\r\n";
  buf += "<hr align=\"center\" width=\"430\" size=\"3\" color=black>\r\n";


  buf += "<BR><BR>\r\n";
  buf += "<hr align=\"center\" width=\"430\" size=\"3\" color=black>\r\n";


  buf += "</body>\r\n";
  buf += "</html>\r\n";

  server.send(200, "text/html", buf);


  saveConfig();
    
}
