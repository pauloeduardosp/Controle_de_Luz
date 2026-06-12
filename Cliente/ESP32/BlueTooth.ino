#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <PubSubClient.h> // Biblioteca para comunicação MQTT

// ================= CONFIGURAÇÕES WI-FI =================
const char* ssid     = "anonymous2";
const char* password = "manuela";

// ================= CONFIGURAÇÕES MQTT =================
const char* mqtt_server   = "iotpaulo.ddne.net";
const int mqtt_port       = 1883;
const char* mqtt_user     = "iotpaulo";
const char* mqtt_pass     = "10tpaulo";
const char* mqtt_topic    = "ape/xiaomi_novo/term"; // TÓPICO ATUALIZADO

WiFiClient espClient;
PubSubClient client(espClient); // Instância do cliente MQTT

int scanTime = 5; 
BLEScan* pBLEScan;
BLEUUID xiaomiServiceUUID((uint16_t)0xFE95);

// Função responsável por garantir a conexão contínua com o Broker MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    // Cria um ID de cliente único baseado no MAC do próprio ESP32
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    
    // Tenta conectar passando ID, usuário e senha fornecidos
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Conectado ao Broker!");
    } else {
      Serial.print("Falha na conexão. Status erro = ");
      Serial.print(client.state());
      Serial.println(". Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

// Classe de captura BLE integrada para envio MQTT
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      String macAddress = advertisedDevice.getAddress().toString();
      macAddress.toLowerCase();
      
      // 1. Filtra pelo prefixo MAC dos termômetros Xiaomi
      if (macAddress.startsWith("a4:c1:38")) {
        
        // 2. Verifica se possui dados sob o serviço 0xFE95
        if (advertisedDevice.haveServiceData()) {
          int serviceDataCount = advertisedDevice.getServiceDataCount();
          
          for (int i = 0; i < serviceDataCount; i++) {
            if (advertisedDevice.getServiceDataUUID(i).equals(xiaomiServiceUUID)) {
              
              String serviceDataStr = advertisedDevice.getServiceData(i);
              size_t length = serviceDataStr.length();
              
              if (length > 0) {
                // Converte os bytes do Service Data para string Hexadecimal pura
                String bytesBrutos = "";
                for (size_t j = 0; j < length; j++) {
                  uint8_t byteValue = (uint8_t)serviceDataStr[j];
                  if (byteValue < 0x10) bytesBrutos += "0";
                  bytesBrutos += String(byteValue, HEX);
                }
                
                // Monta o JSON idêntico ao payload estruturado no seu Raspberry Pi
                String payloadJson = "{\"mac\": \"" + macAddress + "\", \"bytes_brutos\": \"" + bytesBrutos + "\"}";
                
                Serial.println("==================================================");
                Serial.print("Publicando no MQTT: ");
                Serial.println(payloadJson);
                
                // Garante que o cliente está online antes de enviar os dados
                if (!client.connected()) {
                  reconnect();
                }
                
                // Publica a mensagem no tópico configurado
                if(client.publish(mqtt_topic, payloadJson.c_str())) {
                  Serial.println("[MQTT] Sucesso ao publicar pacote!");
                } else {
                  Serial.println("[MQTT] Erro ao publicar pacote.");
                }
                Serial.println("==================================================");
              }
            }
          }
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 1. Inicializa o Wi-Fi
  Serial.println();
  Serial.print("Conectando à rede: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Conectado!");

  // 2. Configura os parâmetros do servidor MQTT
  client.setServer(mqtt_server, mqtt_port);

  // 3. Inicializa o Bluetooth (BLE)
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); 
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); 
}

void loop() {
  // Mantém a conexão MQTT ativa e processa pacotes pendentes de rede
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Serial.println("Iniciando varredura BLE...");
  BLEScanResults* foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults(); 
  
  delay(2000); 
}
