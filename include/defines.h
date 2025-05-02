/*============================================================================================
 *                           PROGRAMA DE CONTROLE E MONITORAMENTO IOT
 *                                    AUTOR: DANILO ANDRADE
 ==============================================================================================*/
#pragma once
#include <Arduino.h>
#include <WiFi.h>   
#include <PubSubClient.h>  
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include "ioSetups.h"
#include "mqttConnection.h"
#include "wifiConnection.h"
#include "functions.h"
#include "attJson.h"

#define TOPICO_SUBSCRIBE "Ceara_Sub"  
#define TOPICO_PUBLISH  "Ceara_Pub"  

#define ID_MQTT "ceara"    //id mqtt (para identificação de sessão) SEMPRE MODIFICAR ESSE TOPICO

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

/*============================================================================================
*                              VARIÁVEIS E OBJETOS GLOBAIS
==============================================================================================*/

extern WiFiClientSecure espClient;;          
extern PubSubClient MQTT;  

extern const char* SSID; 
extern const char* PASSWORD; 

extern const char* BROKER_MQTT; 
extern int BROKER_PORT; 
extern const char* USERNAME; 
extern const char* PASSWORD_MQTT; 

extern int central;        
extern int tempo;
extern int emergencia;
extern int automatico;
extern int RST;
extern int statecompressor1;
extern int statecompressor2;
extern int usina;
extern int compressorINPUT;
extern int compressorINPUT2;

extern const char* hospital;
extern float rede;
extern float dewpoint;
extern float vacuum;
extern float pressure;

extern const unsigned long interval;
extern bool state;


