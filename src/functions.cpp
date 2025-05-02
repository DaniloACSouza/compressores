#include "defines.h"

void compareState(int leitura_saida, String nome) {
    switch (leitura_saida) {
        case 0:
            Serial.println("- Estado do " + nome + " enviado ao broker!");
            Serial.println(leitura_saida);
            break;
        case 1:
            Serial.println("- Estado do " + nome + " enviado ao broker!");
            Serial.println(leitura_saida);
            break;
        default:
            break;
    }
}

String compareAndControlLed(uint8_t pino, int leitura_entrada) {
    switch (leitura_entrada) {
        case 0:
            digitalWrite(pino, LOW);
            
            Serial.println("- Estado da saida " + String(pino) + " enviado ao broker!");
            Serial.println(leitura_entrada);
            return "Desligado";
        case 1:
            digitalWrite(pino, HIGH);
            Serial.println("- Estado da saida " + String(pino) + " enviado ao broker!");
            Serial.println(leitura_entrada);
            return "Ligado";
        default:
            return "Default";
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