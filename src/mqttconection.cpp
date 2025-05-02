#include "defines.h"


void initMQTT() {
    espClient.setInsecure(); // Aceita qualquer certificado
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);         //atribui função de callback 
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  //obtem a string do payload recebido
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }

  if (msg.equals("On")) {
    central = 1;
  }

  if (msg.equals("Off")) {
    central = 0;
  }
  Serial.print(central);
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT, USERNAME, PASSWORD_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Havera nova tentatica de conexao em 2s");
      delay(2000);
    }
  }
}

void VerifyWIFIAndMQTT(void) {
    if (!MQTT.connected())
      reconnectMQTT();  //se não há conexão com o Broker, a conexão é refeita
  
    reconectWiFi();  //se não há conexão com o WiFI, a conexão é refeita
  }
  
void SendOutputStateMQTT(void) {
    compareState(statecompressor1);
    compareState(statecompressor2);
    
    }
  
