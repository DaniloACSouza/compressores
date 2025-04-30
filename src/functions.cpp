#include "defines.h"

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

int readAnalogAverage(int pin) {
    long sum = 0;  //long para evitar overflow
    for (int i = 0; i < NUM_READINGS; i++) {
        sum += analogRead(pin);
        delay(10);  // Pequeno atraso para permitir estabilização
    }
    return sum / NUM_READINGS;  // Retorna a média
  }