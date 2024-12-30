/*============================================================================================
 *                           PROGRAMA DE CONTROLE E MONITORAMENTO IOT
 *                                    AUTOR: DANILO ANDRADE
 ==============================================================================================*/

#include <Arduino.h>
#include <WiFi.h>   // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h>  // Importa a Biblioteca PubSubClient
#include <WiFiClientSecure.h>
/*============================================================================================
 *                   DEFINES DE ID MQTT E TÓPICOS PARA PUBLICAÇÃO E SUBSCRIBE
 ==============================================================================================*/

#define TOPICO_SUBSCRIBE "USINA_ON/OFF_STA_RITA"   //tópico MQTT de escuta
#define TOPICO_PUBLISH  "PRESSAO_USINA_STA_RITA"  //tópico MQTT de envio de informações para Broker
#define TOPICO_PUBLISH1 "COMPRESSOR_1_STATUS_USINA_STA_RITA"
#define TOPICO_PUBLISH2 "COMPRESSOR_2_STATUS_STA_RITA"
#define TOPICO_PUBLISH3 "CONCENTRACAO_O2_USINA_STA_RITA"
#define TOPICO_PUBLISH4 "OGP_CR_STATUS_USINA_STA_RITA"
#define TOPICO_PUBLISH5 "BLCRT_O2_E_USINA_STA_RITA"
#define TOPICO_PUBLISH6 "BLCRT_O2_D_USINA_STA_RITA"
#define TOPICO_PUBLISH7 "RFF_USINA_STA_RITA"
#define TOPICO_PUBLISH8 "BE_USINA_STA_RITA"
#define TOPICO_PUBLISH9 "COMPRESSOR_1_ON/OFF_STA_RITA"
#define TOPICO_PUBLISH10 "STATUS_USINA_STA_RITA"
#define TOPICO_PUBLISH11 "COMPRESSOR_2_ON/OFF_STA_RITA"





/*============================================================================================
 *                                 ID DO MODULO PARA O SERVIDOR MQTT
 *                  SEMPRE MODIFICAR PARA NÃO DAR CONFLITO ENTRE AS PLACAS INSTALADAS
 ==============================================================================================*/

#define ID_MQTT "USINA_CR_NATAL"    //id mqtt (para identificação de sessão) SEMPRE MODIFICAR ESSE TOPICO

/*============================================================================================
 *                             MAPEAMENTO DOS PINOS DO NODEMCU
 ==============================================================================================*/

#define D0 26                                        //DEFINE A ENTRADA DO FALTA DE FASE
#define D1 27                                        //DEFINE A ENTRADA DO MODO AUTOMATICO
#define D2 14                                        //DEFINE A ENTRADA DO BOTAO DE EMERGENCIA
#define D3 12                                        
#define D4 13                                        //DEFINE A ENTRADA DO RELE CICLICO

#define D5 15                                       //DEFINE SAIDA DO COMPRESSOR 1
#define D6 04                                       //DEFINE SAIDA DA OGP
#define D7 16                                       //LED COMPRESSOR 1
#define D8 05                                       //LED OGP
#define D9 18                                       //ALARME
#define D10 02                                        //DEFINE A SAIDA DO COMPRESSOR 2
#define D11 17                                        //LED DO COMPRESSOR 2

#define A0 35                                       //DEFINE A ENTRADA DO SENSOR DOS COMPRESSORES
#define A1 34                                       //DEFINE A ENTRADA DO SENSOR DE REDE
#define A2 39                                       //DEFINE A ENTRADA DO SENSOR DE OXIGENIO
#define A3 36

#define NUM_READINGS 10  // Número de leituras a serem feitas para calcular a média
/*============================================================================================
 *                                         WIFI
 ==============================================================================================*/
 
const char* SSID = "4G-MIFI-6AC"; // SSID da rede WI-FI
const char* PASSWORD = "1234567890"; // Senha da rede WI-FI

/*============================================================================================
 *                                         MQTT
 ==============================================================================================*/
 
const char* BROKER_MQTT = "9281d6a0da6d4dc6a8bc4ce9498e7c52.s1.eu.hivemq.cloud"; // URL do broker MQTT
int BROKER_PORT = 8883; // Porta do Broker MQTT para conexões seguras
const char* USERNAME = "hivemq.webclient.1730054965974"; // Substitua pelo seu usuário
const char* PASSWORD_MQTT = "l9s1@,3KXprVuN.BJ6&n"; // Substitua pela sua senha

/*============================================================================================
 *                              VARIÁVEIS E OBJETOS GLOBAIS
 ==============================================================================================*/

WiFiClientSecure espClient;;          // Cria o objeto espClient
PubSubClient MQTT(espClient);  // Instancia o Cliente MQTT passando o objeto espClient

float leitura;
float pressao;
float direito;
float esquerdo;
float oxigenio;
float concentracao;
float cilindroEsq;
float cilindroDir;

int TEMP;
int TEMP1;
int TEMP2;
int TEMP3;
char timestamp[] = "";
char timestamp1[] = "";
char timestamp2[] = "";
char timestamp3[] = "";

char payload[23] = "000.00";
char payload1[23] = "000.00";
char payload2[23] = "000.00";
char payload3[23] = "000.00";


int central = '0';        //variável que armazena o estado atual da saída
int tempo = 0;
int emergencia = 0;
int automatico = 0;
int RST = 0;
int statecompressor1 = '0';
int statecompressor2 = '0';
int usina = '0';
int compressorINPUT = 0;
int compressorINPUT2 = 0;


// Função para calcular a média das leituras analógicas
int readAnalogAverage(int pin) {
    long sum = 0;  // Usamos long para evitar overflow
    for (int i = 0; i < NUM_READINGS; i++) {
        sum += analogRead(pin);
        delay(10);  // Pequeno atraso para permitir estabilização
    }
    return sum / NUM_READINGS;  // Retorna a média
}


// Intervalo de tempo em milissegundos (8 horas)
const unsigned long interval = 2 * 60 * 60 * 1000; // 8 horas

// Variáveis para controle de tempo
unsigned long previousMillis = 0; // Armazena o último tempo de ativação
bool state = LOW; // Estado atual das saídas

//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);
void InitInput(void);
void EnviaEstadoOutputMQTT(void);