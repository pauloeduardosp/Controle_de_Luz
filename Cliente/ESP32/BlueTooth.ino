#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <PubSubClient.h>
#include <map>

// ================= CONFIGURAÇÕES WI-FI =================
const char* ssid     = "anonymous2";
const char* password = "manuela."; // Mantido com o ponto final do seu código

// ================= CONFIGURAÇÕES MQTT =================
const char* mqtt_server   = "iotpaulo.ddns.net"; // Atualizado conforme seu código (.ddns)
const int mqtt_port       = 1883;
const char* mqtt_user     = "iotpaulo";
const char* mqtt_pass     = "10tpaulo";
const char* mqtt_topic    = "ape/xiaomi_novo/term";

WiFiClient espClient;
PubSubClient client(espClient);

int scanTime = 5; 
BLEScan* pBLEScan;
BLEUUID xiaomiServiceUUID((uint16_t)0xFE95);

// Estrutura para manter o último estado conhecido de cada dado por MAC
struct XiaomiData {
  String ultimo_temp_umi = "";
  String ultima_bateria   = "";
};

std::map<String, XiaomiData> tabelaDispositivos;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Conectado ao Broker!");
    } else {
      Serial.print("Falha. Erro = ");
      Serial.print(client.state());
      Serial.println(". Nova tentativa em 5 segundos...");
      delay(5000);
    }
  }
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      String macAddress = advertisedDevice.getAddress().toString();
      macAddress.toLowerCase();
      
      if (macAddress.startsWith("a4:c1:38")) {
        if (advertisedDevice.haveServiceData()) {
          int serviceDataCount = advertisedDevice.getServiceDataCount();
          
          for (int i = 0; i < serviceDataCount; i++) {
            if (advertisedDevice.getServiceDataUUID(i).equals(xiaomiServiceUUID)) {
              
              String serviceDataStr = advertisedDevice.getServiceData(i);
              size_t length = serviceDataStr.length();
              
              if (length > 0) {
                String bytesBrutos = "";
                for (size_t j = 0; j < length; j++) {
                  uint8_t byteValue = (uint8_t)serviceDataStr[j];
                  if (byteValue < 0x10) bytesBrutos += "0";
                  bytesBrutos += String(byteValue, HEX);
                }
                
                // Armazena seletivamente no array indexado baseado no tamanho real do pacote
                if (length == 15) {
                  tabelaDispositivos[macAddress].ultima_bateria = bytesBrutos;
                  Serial.println("[ESP32 Memória] Bateria atualizada para " + macAddress);
                } else if (length == 17) {
                  tabelaDispositivos[macAddress].ultimo_temp_umi = bytesBrutos;
                  Serial.println("[ESP32 Memória] Temp/Umi atualizada para " + macAddress);
                }

                // PUBLICAÇÃO FORMATADA: Enviamos no formato exato que a "function 4" do Node-RED quer ler
                String payloadJson = "{\"mac\": \"";
                payloadJson += macAddress;
                payloadJson += "\", \"bytes_brutos\": \"";
                payloadJson += bytesBrutos; // Mantém a compatibilidade com o script do Node-RED
                payloadJson += "\"}";
                
                Serial.println("==================================================");
                Serial.print("Enviando para Node-RED: ");
                Serial.println(payloadJson);
                
                if (!client.connected()) {
                  reconnect();
                }
                
                if(client.publish(mqtt_topic, payloadJson.c_str())) {
                  Serial.println("[MQTT] Publicado!");
                } else {
                  Serial.println("[MQTT] Erro ao publicar.");
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

  Serial.println();
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Conectado!");

  client.setServer(mqtt_server, mqtt_port);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); 
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Serial.println("Iniciando varredura BLE...");
  BLEScanResults* foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults(); 
  
  delay(10000); 
}
