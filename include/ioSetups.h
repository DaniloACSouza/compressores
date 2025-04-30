#pragma once
#include <Arduino.h>
#include "defines.h"

void InitOutput(void) {
    pinMode(OUT_COMPRESSOR, OUTPUT); //COMPRESSOR 01
    pinMode(D6, OUTPUT); //OGP
    pinMode(D7, OUTPUT); //XC1
    pinMode(LED_OGP, OUTPUT); //XOGP
    pinMode(ALARM, OUTPUT); //ALARME
    pinMode(RELAY_COMPRESSOR2, OUTPUT);//COMPRESSOR 02
    pinMode(LED_COMPRESSOR2, OUTPUT);//XC2
  }
  void InitInput(void) {  
    pinMode(SUPERVISORIO, INPUT);  //FALTA DE FASE
    pinMode(AUTOMATICO, INPUT);  //CHAVE LIGA AUTOMATICO
    pinMode(EMERGENCY_BUTTON, INPUT);  //BOTAO DE EMERGENCIA   
    pinMode(COMPRESSOR_INPUT, INPUT);  //COMPRESSOR 1 ENTRADA DIGITAL
    pinMode(D4, INPUT);  //COMPRESSOR 2 ENTRADA DIGITAL
  }