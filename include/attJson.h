#pragma once
#include "defines.h"
#include <ArduinoJson.h>


inline void sendJson() {
    DynamicJsonDocument doc(512);
    doc.clear();

    doc["Hospital"] = hospital;
    doc["C1"] = statecompressor1;
    doc["C2"] = statecompressor2;
    doc["BE"] = emergencia;
    doc["RST"] = RST;
    doc["auto"] = automatico;
    doc["rede"] = rede;
    doc["pressure"] = pressure;
    doc["dew_point"] = dewpoint;
    doc["vacuo"] = vacuum;
    
    String jsonpronto;
    serializeJson(doc, jsonpronto);

    Serial.println(jsonpronto);
    MQTT.publish(TOPICO_PUBLISH, jsonpronto.c_str());
}
