/*============================================================================================
 *                           PROGRAMA DE CONTROLE E MONITORAMENTO IOT
 *                                    AUTOR: DANILO ANDRADE
 ==============================================================================================*/
#pragma once
#include <Arduino.h>
#include <WiFi.h>   // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h>  // Importa a Biblioteca PubSubClient
#include <WiFiClientSecure.h>
/*============================================================================================
 *                   DEFINES DE ID MQTT E TÓPICOS PARA PUBLICAÇÃO E SUBSCRIBE
 ==============================================================================================*/

#define TOPICO_SUBSCRIBE "USINA_ON/OFF_STA_RITA"   //tópico MQTT de escuta
#define TOPICO_PUBLISH  "PRESSAO_USINA_STA_RITA"  //tópico MQTT de envio de informações para Broker
// #define TOPICO_PUBLISH1 "COMPRESSOR_1_STATUS_USINA_STA_RITA"
// #define TOPICO_PUBLISH2 "COMPRESSOR_2_STATUS_STA_RITA"
// #define TOPICO_PUBLISH3 "CONCENTRACAO_O2_USINA_STA_RITA"
// #define TOPICO_PUBLISH4 "OGP_CR_STATUS_USINA_STA_RITA"
// #define TOPICO_PUBLISH5 "BLCRT_O2_E_USINA_STA_RITA"
// #define TOPICO_PUBLISH6 "BLCRT_O2_D_USINA_STA_RITA"
// #define TOPICO_PUBLISH7 "RFF_USINA_STA_RITA"
// #define TOPICO_PUBLISH8 "BE_USINA_STA_RITA"
// #define TOPICO_PUBLISH9 "COMPRESSOR_1_ON/OFF_STA_RITA"
// #define TOPICO_PUBLISH10 "STATUS_USINA_STA_RITA"
// #define TOPICO_PUBLISH11 "COMPRESSOR_2_ON/OFF_STA_RITA"
// #define TOPICO_12 "JSON_GERAL"





/*============================================================================================
 *                                 ID DO MODULO PARA O SERVIDOR MQTT
 *                  SEMPRE MODIFICAR PARA NÃO DAR CONFLITO ENTRE AS PLACAS INSTALADAS
 ==============================================================================================*/

#define ID_MQTT "USINA_CR_NATAL"    //id mqtt (para identificação de sessão) SEMPRE MODIFICAR ESSE TOPICO

/*============================================================================================
 *                             MAPEAMENTO DOS PINOS DO NODEMCU
 ==============================================================================================*/

#define SUPERVISORIO 26                                        
#define AUTOMATIC 27                                        
#define EMERGENCY_BUTTON 14                                        

#define COMPRESSOR_INPUT1 12                                        
#define COMPRESSOR_INPUT2 13                                        

#define RELAY_COMPRESSOR1 15                                      
#define RELAY_COMPRESSOR2 2                                       
#define OGP_OUT 4

#define LED_COMPRESSOR1 16                                      
#define LED_COMPRESSOR2 17                                        
#define LED_OGP 5                                       
#define ALARM 18                                      

#define REDE 35                                       
#define PRESSURE 34                                       
#define DEW_POINT 39                                      
#define VACUUM 36

#define NUM_READINGS 10  // Número de leituras a serem feitas para calcular a média

const char* SSID = "4G-MIFI-6AC"; 
const char* PASSWORD = "1234567890"; 


const char* BROKER_MQTT = "9281d6a0da6d4dc6a8bc4ce9498e7c52.s1.eu.hivemq.cloud"; 
int BROKER_PORT = 8883; 
const char* USERNAME = "hivemq.webclient.1730054965974"; 
const char* PASSWORD_MQTT = "l9s1@,3KXprVuN.BJ6&n"; 

/*============================================================================================
 *                              VARIÁVEIS E OBJETOS GLOBAIS
 ==============================================================================================*/

WiFiClientSecure espClient;;          
PubSubClient MQTT(espClient);  

// float leitura;
// float pressao;
// float direito;
// float esquerdo;
// float oxigenio;
// float concentracao;
// float cilindroEsq;
// float cilindroDir;

const char* hospital = "ceara";
float rede;
float dewpoint;
float vacuum;
float pressure;

// int TEMP;
// int TEMP1;
// int TEMP2;
// int TEMP3;
// char timestamp[] = "";
// char timestamp1[] = "";
// char timestamp2[] = "";
// char timestamp3[] = "";
// char timestamp4[] = "";

// char payload[23] = "000.00";
// char payload1[23] = "000.00";
// char payload2[23] = "000.00";
// char payload3[23] = "000.00";
// char payload4[23] = "";


int readAnalogAverage(int pin) {
    long sum = 0;  //long para evitar overflow
    for (int i = 0; i < NUM_READINGS; i++) {
        sum += analogRead(pin);
        delay(10);  // Pequeno atraso para permitir estabilização
    }
    return sum / NUM_READINGS;  // Retorna a média
}


// Intervalo de tempo em milissegundos (8 horas)
const unsigned long interval = 8 * 60 * 60 * 1000; 

// Variáveis para controle de tempo
unsigned long previousMillis = 0; // Armazena o último tempo de ativação
bool state = LOW; // Estado atual das saídas

//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerifyWIFIAndMQTT(void);
void InitOutput(void);
void InitInput(void);
void SendOutputStateMQTT(void);