/*
 * Cliente
 *
 *    v3.5.5 - versão estavel apos recuperação dos arquivos perdidos
 *    v3.5.6 - autenticação no usuario mqtt
 */   
 
#include <EEPROM.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>    // mqtt
//#include <ArduinoJson.h>

 

#define EEPROM_tipoDevice 0           // tamanho total 1 byte
#define EEPROM_gpioPrimario 1         // tamanho total 1 byte
#define EEPROM_gpioSecundario 2       // tamanho total 1 byte
#define EEPROM_qtdLuz 3               // tamanho total 1 byte
#define EEPROM_totalReset 4           // tamanho total 1 byte
#define EEPROM_qtdReset 5             // tamanho total 1 byte
//#define EEPROM_idLuz2 6             // tamanho total 1 byte
//#define EEPROM_idLuz3 7             // tamanho total 1 byte
#define EEPROM_gpioPrimario0 8        // tamanho total 1 byte
#define EEPROM_gpioPrimario1 9        // tamanho total 1 byte
#define EEPROM_gpioPrimario2 10       // tamanho total 1 byte
#define EEPROM_gpioPrimario3 11       // tamanho total 1 byte
#define EEPROM_tipoInterruptor0 12    // tamanho total 1 byte
#define EEPROM_tipoInterruptor1 13    // tamanho total 1 byte
#define EEPROM_tipoInterruptor2 14    // tamanho total 1 byte
#define EEPROM_tipoInterruptor3 15    // tamanho total 1 byte
#define EEPROM_gpioSecundario0 16     // tamanho total 1 byte
#define EEPROM_gpioSecundario1 17     // tamanho total 1 byte
#define EEPROM_gpioSecundario2 18     // tamanho total 1 byte
#define EEPROM_gpioSecundario3 19     // tamanho total 1 byte
#define EEPROM_tipoDispositivo0 20    // tamanho total 1 byte
#define EEPROM_tipoDispositivo1 21    // tamanho total 1 byte
#define EEPROM_tipoDispositivo2 22    // tamanho total 1 byte
#define EEPROM_tipoDispositivo3 23    // tamanho total 1 byte


#define EEPROM_wifi 40                // tamanho total 50 bytes=> 30 p/ ssid e 20 p/ password
#define EEPROM_mqttAtuadorId0 90      // tamanho total 40 bytes
#define EEPROM_mqttAtuadorId1 130     // tamanho total 40 bytes
#define EEPROM_mqttAtuadorId2 170     // tamanho total 40 bytes
#define EEPROM_mqttAtuadorId3 210     // tamanho total 40 bytes
#define EEPROM_mqttServer 250         // tamanho total 40 bytes
#define EEPROM_mqttUser 290            // tamanho total 20 bytes
#define EEPROM_mqttPass 310            // tamanho total 20 bytes

void http_reset(void);


String versao = "3.5.6";


int gpioPrimario[5] = {1};                //TX GPIO01     aba mudanca_interruptor, rst, ap, verificacao_status_lampada, http_config
int gpioSecundario[5] = {3};        //RX GPI03      aba mudanca_interruptor, http_config
int tipoDispositivo[5] = {0,0,0,0,0};
          // 1 para interruptor
          // 2 para dth11
          // 3 para dth21
int tipoInterruptor[5] = {0, 0, 0, 0};             //aba http_config
          // 0 para não utilizado
          // 1 para paralelo
          // 2 para ligado ao gpio

String mqttDeviceId = "newdevice";
String mqttPlaceId;
char* char_mqttDeviceId = &mqttDeviceId[0u];
String mqttServer;
String mqttUser;
String mqttPass;

String mqttAtuadorId[5] = {"newdevice/atuador0", "newdevice/atuador1","newdevice/atuador2","newdevice/atuador3", "newdevice/atuador4"};


//############################################
//        Variaveis novas


boolean divulgaAP = true;  // aba ap
boolean verificacaoStatusLampada = true;
unsigned long lastMeasureVerificacaoStatusLampada;
byte octeto[4];

//############################################


boolean lastStatusInt[5];          // aba mudanca_interruptor
int delayinterruptor;           // aba mudanca_interruptor
boolean lastStatusLampada[5];      // aba verificacao_status_lampada

// int delayversao = 0;            // aba ap

// int idClient = 0;               //4 octeto do ip, definido na aba ap, verificacao_status_lampada



int qtdLuz = 1;                 //aba http_config




ESP8266WebServer server(82);
ESP8266HTTPUpdateServer httpUpdater;

struct wifi{
  char ssid[30];  //= "anonymous2";
  char password[20]; // = "manuela.";
} wifi;

char *APssid = "newdevice";
char *APpassword= "newdevice";

extern "C" {
#include "user_interface.h"
}


//IPAddress ipServer(0,0,0,0);     // casa SERVER varanda

//######################
//  variaveis MQTT

char* idMqtt = "room/lamp";
char* change = "/change";
char* topicoPublish = "room/lamp";
boolean lastMqttStatus;
const char* mqtt_server = "smcamericas.ddns.net"; 
boolean mqttUpdate = false;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long now = millis();
unsigned long lastMeasure = 0;

unsigned long tempoUpdate;
// final
//######################

//######################
// dht

#include "DHT.h"

unsigned long ultimaMedicaoDht;
unsigned long ultimoEnvioDht;
DHT *dht;

int qtdDht = 0;

float v_umidade[30];
float v_temperatura[30];
    
// final
//######################
 

void setup() {
//  Serial.begin(115200);            // Inicial Serial
  Serial.begin(74880);            // Inicial Serial
  
  EEPROM.begin(512);              // inicial EEPROM


//#########################
//### Função para reset constante
  EEPROM.write(EEPROM_qtdReset, EEPROM.read(EEPROM_qtdReset)+1);
  Serial.print("EEPROM_qtdReset ");
  Serial.println(EEPROM.read(EEPROM_qtdReset));

  if(EEPROM.read(EEPROM_qtdReset) >= 5){
    
    Serial.println("####### REALIZANDO ERASER NA EEPROM ##########") ;
    for(int p = 1; p <= 512; p++){
      EEPROM.write(p, 255);
    }

  }

  EEPROM.write(EEPROM_totalReset, EEPROM.read(EEPROM_totalReset)+1);
  EEPROM.commit();  

//### Fim Função para reset constante
//########################

  loadConfig();                   // faz load das configuaçoes gravadas na eeprom para as variaveis

  dhtsetup();
  mqtt_setup();
  char* wifi_hostname = &mqttDeviceId[0u];
  wifi_station_set_hostname(wifi_hostname);

  httpUpdater.setup(&server);
  server.on("/reset",http_reset);                     // h_r
  server.on("/config", http_config);  //pagina http de confiugaracao do modulo - aba http_config
  server.on("/configpost", HTTP_POST, http_config_post);  //pagina http de confiugaracao do modulo - aba http_config

  server.begin();


  for(int p = 0; p < qtdLuz; p++){
    if(tipoDispositivo[p] == 1){
      pinMode(gpioPrimario[p], OUTPUT);
      digitalWrite(gpioPrimario[p], HIGH); // inicializa com a lampada desligada
    } else if (tipoDispositivo[p] == 2 || tipoDispositivo[p] == 3) {
      pinMode(gpioPrimario[p], INPUT);
    }

    if(tipoInterruptor[p] == 1 || tipoInterruptor[p] == 2){
      pinMode(gpioSecundario[p], INPUT);  
    }
    
  }




//  WiFi.begin("anonymous2", "manuela.");
  WiFi.begin(wifi.ssid, wifi.password); 
  WiFi.softAP(APssid); //, APpassword);


//  IPAddress ipServer(octeto[0],octeto[1],octeto[2],octeto[3]);
//  String value = ipServer.toString();


  EEPROM.write(EEPROM_qtdReset, 0);       // função reset constante
  EEPROM.commit();                        // função reset constante



}


void loop() {

dht_loop();
  mudanca_interruptor();          // monitora o status do interruptor - aba mudança_interruptor
  server.handleClient();          // referente a funçao server.on
  verificacao_status_lampada();   // monitora o status da lampada - aba verificacao_status_lampada
  wifi_ap();               // envia update inicial do modulo - aba a
mqttEspStatus();

 if (!client.connected() || !lastMqttStatus) {
    
    reconnect();
    
  }
  client.loop();    /// comentar linha caso necessario descomentar linhas abaixo



}
