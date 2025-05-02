#include "defines.h"

void sendJson() {
    JsonDocument doc;
    doc.clear();

    doc["Hospital"] = hospital;
    doc["C1"] = compressor1estado;
    doc["C2"] = compressor2estado;
    doc["BE"] = emergency_button;
    doc["RST"] = supervisorio;
    doc["auto"] = man_auto;
    doc["rede"] = rede;
    doc["pressure"] = pressure;
    doc["dew_point"] = dewpoint;
    doc["vacuo"] = vacuum;
    
    String jsonpronto;
    serializeJson(doc, jsonpronto);

    Serial.println(jsonpronto);
    MQTT.publish(TOPICO_PUBLISH, jsonpronto.c_str());
}
