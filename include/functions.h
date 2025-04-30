#pragma once
#include "defines.h"
#include "attJson.h"

void compareState(int leitura_saida) {
    switch (leitura_saida) {
        case '0':
            sendJson();
            Serial.println("- Estado do " + String(leitura_saida) + " enviado ao broker!");
            Serial.println(leitura_saida);
            break;
        case '1':
            sendJson();
            Serial.println("- Estado do " + String(leitura_saida) + " enviado ao broker!");
            Serial.println(leitura_saida);
            break;
        default:
            break;
    }
}

void compareAndControl(uint8_t pino, int leitura_saida) {
    switch (leitura_saida) {
        case '0':
            sendJson();
            digitalWrite(pino, LOW);
            Serial.println("- Estado do " + String(leitura_saida) + " enviado ao broker!");
            Serial.println(leitura_saida);
            break;
        case '1':
            sendJson();
            digitalWrite(pino, HIGH);
            Serial.println("- Estado do " + String(leitura_saida) + " enviado ao broker!");
            Serial.println(leitura_saida);
            break;
        default:
            break;
    }
}