#include "defines.h"


const char* hospital = "ceara";
int central = 0;        
int tempo = 0;
int emergencia = 0;
int automatico = 0;
int RST = 0;
int statecompressor1 = 0;
int statecompressor2 = 0;
int usina = 0;
int compressorINPUT = 0;
int compressorINPUT2 = 0;

const float minimumPressure = 6.00;
const float maximumPressure = 8.00;

float rede;
float dewpoint;
float vacuum;
float pressure;

const char* SSID = "ORION"; 
const char* PASSWORD = "MK79RV23PQ"; 

const char* BROKER_MQTT = "9281d6a0da6d4dc6a8bc4ce9498e7c52.s1.eu.hivemq.cloud"; 
int BROKER_PORT = 8883; 
const char* USERNAME = "Piaui_on_off"; 
const char* PASSWORD_MQTT = "Da32525291"; 

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

  compareAndControlLed(LED_COMPRESSOR1, compressorINPUT);
  compareAndControlLed(LED_COMPRESSOR2, compressorINPUT2);
  

  if (central == 1) {
    digitalWrite(OGP_OUT, HIGH);
    digitalWrite(LED_OGP, HIGH);
  }
  if (RST == 1) {
    digitalWrite(ALARM, LOW);
  }
  if (emergencia == 1) {
    digitalWrite(ALARM, LOW);
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
 
  if ((automatico == 1) && (pressure < minimumPressure)) {
    digitalWrite(RELAY_COMPRESSOR1, state ? HIGH : LOW);
    digitalWrite(RELAY_COMPRESSOR2, state ? LOW : HIGH);
    }

  if ((automatico == 1) && (pressure > maximumPressure)) {
    digitalWrite(RELAY_COMPRESSOR1, LOW);
    digitalWrite(RELAY_COMPRESSOR2, LOW);
  }
  

  if (automatico == 0) {
    digitalWrite(RELAY_COMPRESSOR1, LOW);
    digitalWrite(RELAY_COMPRESSOR2, LOW);
  }
  if (emergencia == 0) {
    digitalWrite(ALARM, HIGH);
    digitalWrite(RELAY_COMPRESSOR1, LOW);
    digitalWrite(RELAY_COMPRESSOR2, LOW);

  }
  if (RST == 0) {
    digitalWrite(ALARM, HIGH);
  }
  if (central == 0) {
    // digitalWrite(OUT_COMPRESSOR1, LOW);
    // digitalWrite(D7, LOW);
      digitalWrite(OGP_OUT, LOW);
      digitalWrite(LED_OGP, LOW);
  

    }

  rede = ((readAnalogAverage(REDE) -590) * 20 / 4095);
  pressure = ((readAnalogAverage(PRESSURE) -590) * 20 / 4095);
  dewpoint = ((readAnalogAverage(DEW_POINT) - 590) * 20 / 4095);
  vacuum = ((readAnalogAverage(VACUUM) - 590) * 20 / 4095);
  sendJson();
  VerifyWIFIAndMQTT();
  SendOutputStateMQTT();
  delay(5000);
  MQTT.loop();
  
}