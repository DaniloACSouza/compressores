#pragma once

void initMQTT();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT();
void VerifyWIFIAndMQTT(void);
void SendOutputStateMQTT(void);