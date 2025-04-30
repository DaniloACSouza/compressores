#include "defines.h"
#include <unity.h>

void TESTE_SEND_JSON() {
    sendJson();
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(TESTE_SEND_JSON);
    UNITY_END();
}

void loop() {}