#include "defines.h"


const char* hospital = "ceara";
int central = '0';        
int tempo = 0;
int emergencia = 0;
int automatico = 0;
int RST = 0;
int statecompressor1 = '0';
int statecompressor2 = '0';
int usina = '0';
int compressorINPUT = 0;
int compressorINPUT2 = 0;

float rede;
float dewpoint;
float vacuum;
float pressure;

const char* SSID = "4G-MIFI-6AC"; 
const char* PASSWORD = "1234567890"; 

const char* BROKER_MQTT = "9281d6a0da6d4dc6a8bc4ce9498e7c52.s1.eu.hivemq.cloud"; 
int BROKER_PORT = 8883; 
const char* USERNAME = "hivemq.webclient.1730054965974"; 
const char* PASSWORD_MQTT = "l9s1@,3KXprVuN.BJ6&n"; 

// Intervalo de tempo em milissegundos (8 horas)
const unsigned long interval = 8 * 60 * 60 * 1000; 

// Variáveis para controle de tempo
unsigned long previousMillis = 0; // Armazena o último tempo de ativação
bool state = LOW; // Estado atual das saídas

WiFiClientSecure espClient;;          
PubSubClient MQTT(espClient);  

void setup() {
  Serial.begin(115200);
  InitOutput();
  initWiFi();
  initMQTT();

  digitalWrite(RELAY_COMPRESSOR1, HIGH);
  digitalWrite(RELAY_COMPRESSOR2, LOW);
}

void loop() {
  statecompressor1 = digitalRead(RELAY_COMPRESSOR1);
  statecompressor2 = digitalRead(RELAY_COMPRESSOR2);
  usina = digitalRead(OGP_OUT);
  automatico = digitalRead(AUTOMATIC);
  RST = digitalRead(SUPERVISORIO);
  emergencia = digitalRead(EMERGENCY_BUTTON);
  compressorINPUT = digitalRead(COMPRESSOR_INPUT1);
  compressorINPUT2 = digitalRead(COMPRESSOR_INPUT2);

  Serial.println("RST: ");
  Serial.println(RST);  
  Serial.println("automatico: ");
  Serial.println(automatico);
  Serial.println("emergencia: ");
  Serial.println(emergencia);

  compareAndControl(LED_COMPRESSOR1, compressorINPUT);
  compareAndControl(LED_COMPRESSOR2, compressorINPUT2);
  

  if (central == '1') {
    digitalWrite(OGP_OUT, HIGH);
    digitalWrite(LED_OGP, HIGH);
    sendJson();
  }
  if (RST == 1) {
    digitalWrite(ALARM, LOW);
    sendJson();
  }
  if (emergencia == 1) {
    digitalWrite(ALARM, LOW);
    sendJson();
  }
  
//START TIMER
  unsigned long currentMillis = millis(); // Obtém o tempo atual
// Verifica se o intervalo de 8 horas foi atingido
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Atualiza o tempo anterior
    // Alterna os estados das saídas
    state = !state;
    }
//END TIMER
 
  if ((automatico == 1) && (pressure < 5.00)) {
    digitalWrite(RELAY_COMPRESSOR1, state ? HIGH : LOW);
    digitalWrite(RELAY_COMPRESSOR2, state ? LOW : HIGH);
    }

  if ((automatico == 1) && (pressure > 6.00)) {
    digitalWrite(RELAY_COMPRESSOR1, LOW);
    digitalWrite(RELAY_COMPRESSOR2, LOW);
  }
  

  if (automatico == 0) {
    Serial.println("======== SISTEMA EM MANUAL========");
    Serial.println(" ");
    digitalWrite(RELAY_COMPRESSOR1, LOW);
    digitalWrite(RELAY_COMPRESSOR2, LOW);
  }
  if (emergencia == 0) {
    Serial.println("======= BOTÃO DE EMERGENCIA ACIONADO =======");
    Serial.println(" ");
    digitalWrite(ALARM, HIGH);
    digitalWrite(RELAY_COMPRESSOR1, LOW);
    digitalWrite(RELAY_COMPRESSOR2, LOW);
    sendJson();

  }
  if (RST == 0) {
    Serial.println("=========== FALTA DE FASE ==========");
    Serial.println(" ");
    digitalWrite(ALARM, HIGH);
    sendJson();

  }
  if (central == '0') {
    // digitalWrite(OUT_COMPRESSOR1, LOW);
    // digitalWrite(D7, LOW);
      digitalWrite(OGP_OUT, LOW);
      digitalWrite(LED_OGP, LOW);
      sendJson();

    }

  rede = ((readAnalogAverage(REDE) -590) * 20 / 4095);
  pressure = ((readAnalogAverage(PRESSURE) -590) * 20 / 4095);
  dewpoint = ((readAnalogAverage(DEW_POINT) - 590) * 20 / 4095);
  vacuum = ((readAnalogAverage(VACUUM) - 590) * 20 / 4095);
  sendJson();
  VerifyWIFIAndMQTT();
  SendOutputStateMQTT();
  MQTT.loop();
  
/*====================================================================================================
*                             MONITORA E ESCREVE O STATUS DOS COMPRESSORES
======================================================================================================*/

  // {
  //   Serial.println("======= STATUS COMPRESSORES =======");
  //   Serial.print("compressor1: ");
  //   Serial.println(statecompressor1);
  //   Serial.print("compressor2: ");
  //   Serial.println(statecompressor2);
  //   Serial.println(" ");
  // }

/*================================================================================================
*                        MONITORA E ESCREVE O STATUS DOS SENSORES NO MONITOR
==================================================================================================*/

  // {
  //   Serial.println("======= NOVA LEITURA =======");
  //   Serial.print("Pressão dos compressores: ");
  //   Serial.println(leitura);
  //   Serial.print("Pressão do lado direito: ");
  //   Serial.println(direito);
  //   Serial.print("concentração de oxigênio: ");
  //   Serial.println(oxigenio);
  //   Serial.println(" ");
  // }

   /*=================================================================================================
  *                 LEITURA E ENVIO DOS DADOS DO SENSOR DE PRESSÃO DOS COMPRESSORES
  ====================================================================================================*/
  // char TEMP[5];
  // String CUR = String(pressao, 1);
  // CUR.toCharArray(TEMP, 4);
  // strcpy(payload, "");
  // strcat(payload, TEMP);
  // strcat(payload, "");
  // strcat(payload, timestamp);
  // strcat(payload, "");
  // Serial.print("Payload pressão dos compressores: ");
  // Serial.println(payload);
  // MQTT.publish(TOPICO_PUBLISH, payload);

  /*=================================================================================================
  *                            LEITURA E ENVIO DOS DADOS DO SENSOR DE OXIGENIO
  ==================================================================================================*/

  // char TEMP1[5];
  // String CUR1 = String(concentracao, 1);
  // CUR1.toCharArray(TEMP1, 4);
  // strcpy(payload1, "");
  // strcat(payload1, TEMP1);
  // strcat(payload1, "");
  // strcat(payload1, timestamp1);
  // strcat(payload1, "");
  // Serial.print("Payload concentração de oxigênio: ");
  // Serial.println(payload1);
  // // MQTT.publish(TOPICO_PUBLISH3, payload1);
  // delay(1000);

  /*=================================================================================================
  *                           LEITURA E ENVIO DOS DADOS DOS SENSORES DO BLOCO CENTRAL
  ====================================================================================================*/

  // char TEMP2[5];
  // String CUR2 = String(cilindroEsq, 3);
  // CUR2.toCharArray(TEMP2, 8);
  // strcpy(payload2, "");
  // strcat(payload2, TEMP2);
  // strcat(payload2, "");
  // strcat(payload2, timestamp2);
  // strcat(payload2, "");
  // Serial.print("Payload pressão do lado esquerdo: ");
  // Serial.println(payload2);
  // MQTT.publish(TOPICO_PUBLISH5, payload2);

/*-----------------------------------------------------------------------------------------------------------------*/
  
  // char TEMP3[5];
  // String CUR3 = String(cilindroDir, 3);
  // CUR3.toCharArray(TEMP3, 8);
  // strcpy(payload3, "");
  // strcat(payload3, TEMP3);
  // strcat(payload3, "");
  // strcat(payload3, timestamp3);
  // strcat(payload3, "");
  // Serial.print("Payload pressão do lado direito: ");
  // Serial.println(payload3);
  // MQTT.publish(TOPICO_PUBLISH6, payload3);

  //garante funcionamento das conexões WiFi e ao broker MQTT

  //envia o status de todos os outputs para o Broker no protocolo esperado

  //keep-alive da comunicação com broker MQTT
}