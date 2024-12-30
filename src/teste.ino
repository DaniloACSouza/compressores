#include <usina_compilado.h>
#include <ArduinoJson.h>

/*======================================================================================================
 *                                      IMPREMENTAÇÃO DAS FUNÇÕES
 ======================================================================================================*/
void setup() {
  //inicializações:
  Serial.begin(115200);
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();

    pinMode(D0, INPUT);  //FALTA DE FASE
    pinMode(D1, INPUT);  //CHAVE LIGA AUTOMATICO
    pinMode(D2, INPUT);  //BOTAO DE EMERGENCIA   
    pinMode(D3, INPUT);  //COMPRESSOR 1 ENTRADA DIGITAL
    pinMode(D4, INPUT);  //COMPRESSOR 2 ENTRADA DIGITAL
    pinMode(D5, OUTPUT); //COMPRESSOR 01
    pinMode(D6, OUTPUT); //OGP
    pinMode(D7, OUTPUT); //XC1
    pinMode(D8, OUTPUT); //XOGP
    pinMode(D9, OUTPUT); //ALARME
    pinMode(D10, OUTPUT);//COMPRESSOR 02
    pinMode(D11, OUTPUT);//XC2

  digitalWrite(D5, HIGH);
  digitalWrite(D10, LOW);

}

/*======================================================================================================
 *                            INICIALIZA A COMUNICAÇÃO SERIAL COM BAURATE 115200 
 =======================================================================================================*/

void initSerial() {
  Serial.begin(115200);
}

/*========================================================================================================
*                           INICIALIZA E CONECTA-SE NA REDE WIFI DESEJADA
=========================================================================================================*/

void initWiFi() {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconectWiFi();
}

/*======================================================================================================
 *                              INICIALIZA PARAMETROS DE CONEXÃO MQTT
 *                        Endereço do broker, porta e seta função de callback
 ======================================================================================================*/

void initMQTT() {
    espClient.setInsecure(); // Aceita qualquer certificado
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);         //atribui função de callback 
}

/*=======================================================================================================
 *                                        FUNÇÃO DE CALLBACK
 *          Esta função é chamada toda vez que uma informação de um dos tópicos subescritos chega
 =======================================================================================================*/

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  //obtem a string do payload recebido
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }

  //toma ação dependendo da string recebida:
  //verifica se deve colocar nivel alto de tensão na saída D0:

  if (msg.equals("L")) {
    central = '1';
  }

  //verifica se deve colocar nivel alto de tensão na saída D0:
  if (msg.equals("D")) {
    central = '0';
  }
  Serial.print(central);
}

/*=======================================================================================================
 *                                  RECONECTA-SE AO BROKER MQTT
 *                  Caso ainda não esteja conectado ou em caso de a conexão cair
 *             Em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
 =======================================================================================================*/

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT, USERNAME, PASSWORD_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Havera nova tentatica de conexao em 2s");
      delay(2000);
    }
  }
}

/*====================================================================================================
*                                       RECONECTA-SE AO WIFI
======================================================================================================*/

void reconectWiFi() {
  //se já está conectado a rede WI-FI, nada é feito.
  //Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD);  // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());
}

/*======================================================================================================
 *                      VERIFICA O ESTADO ATUAL DAS CONEXÕES WIFI E AO BROKER MQTT 
 *                  Em caso de desconexão (qualquer uma das duas), a conexão é refeita.
 =====================================================================================================*/

void VerificaConexoesWiFIEMQTT(void) {
  if (!MQTT.connected())
    reconnectMQTT();  //se não há conexão com o Broker, a conexão é refeita

  reconectWiFi();  //se não há conexão com o WiFI, a conexão é refeita
}

/*=====================================================================================================
*                                ENVIA AO BROKER O ESTADO ATUAL DO OUTPUT
======================================================================================================*/

void EnviaEstadoOutputMQTT(void) {
  if (statecompressor1 == '0')
    MQTT.publish(TOPICO_PUBLISH1, "0");

  if (statecompressor1 == '1')
    MQTT.publish(TOPICO_PUBLISH1, "1");

  Serial.println("- Estado do compressor 1 enviado ao broker!");
    Serial.println(statecompressor1);

  delay(1000);
/*  if (statecompressor2 == '0')
    MQTT.publish(TOPICO_PUBLISH2, "D");

  if (statecompressor2 == '1')
    MQTT.publish(TOPICO_PUBLISH2, "L");

  Serial.println("- Estado do compressor 2 enviado ao broker!");
      Serial.println(statecompressor2);

  delay(1000);
*/
  if (usina == '0')
    MQTT.publish(TOPICO_PUBLISH4, "D");

  if (usina == '1')
    MQTT.publish(TOPICO_PUBLISH4, "L");

  Serial.println("- Estado da usina enviado ao broker!");
      Serial.println(usina);

  delay(1000);
}

/*================================================================================================
 *                           INICIALIZA O OUTPUT EM NIVEL LOGICO BAIXO
 ==================================================================================================*/

void InitOutput(void) {

    pinMode(D5, OUTPUT); //COMPRESSOR 01
    pinMode(D6, OUTPUT); //OGP
    pinMode(D7, OUTPUT); //XC1
    pinMode(D8, OUTPUT); //XOGP
    pinMode(D9, OUTPUT); //ALARME
    pinMode(D10, OUTPUT);//COMPRESSOR 02
    pinMode(D11, OUTPUT);//XC2
}
void InitInput(void) {
    
    pinMode(D0, INPUT);  //FALTA DE FASE
    pinMode(D1, INPUT);  //CHAVE LIGA AUTOMATICO
    pinMode(D2, INPUT);  //BOTAO DE EMERGENCIA
    pinMode(D3, INPUT);  //COMPRESSOR 2 ENTRADA DIGITAL
    pinMode(D4, INPUT);  //COMPRESSOR 1 ENTRADA DIGITAL
}

/*===================================================================================================
*                                         PROGRAMA PRINCIPAL
=====================================================================================================*/

void loop(){

/*==================================================================================================
*                              CONTROLA A ENTRADA E SAIDA DOS COMPRESSORES
====================================================================================================*/

  statecompressor1 = digitalRead(D5);
  statecompressor2 = digitalRead(D10);
  usina = digitalRead(D6);
  automatico = digitalRead(D1);
  RST = digitalRead(D0);
  emergencia = digitalRead(D2);
  compressorINPUT = digitalRead(D3);
  compressorINPUT2 = digitalRead(D4);

//   Serial.println("RST: ");
//   Serial.println(RST);  
//   Serial.println("automatico: ");
//   Serial.println(automatico);
//   Serial.println("emergencia: ");
//   Serial.println(emergencia);

  if (compressorINPUT == 1) {
        //   MQTT.publish(TOPICO_PUBLISH9, "1");
    digitalWrite(D7, HIGH);
   }
  if (compressorINPUT == 0){
        //   MQTT.publish(TOPICO_PUBLISH9, "0");
    digitalWrite(D7, LOW);
    }
  if (compressorINPUT2 == 1) {
        //   MQTT.publish(TOPICO_PUBLISH11, "1");
    digitalWrite(D11, HIGH);
   }
  if (compressorINPUT2 == 0){
        //   MQTT.publish(TOPICO_PUBLISH11, "0");
    digitalWrite(D11, LOW);
    }

  if (central == '1')
  {
    digitalWrite(D6, HIGH);
    digitalWrite(D8, HIGH);
        //   MQTT.publish(TOPICO_PUBLISH10, "1");

  }
  if (RST == 1){
    digitalWrite(D9, LOW);
        // MQTT.publish(TOPICO_PUBLISH7, "1");

  }
  if (emergencia == 1) 
  {
    digitalWrite(D9, LOW);
        // MQTT.publish(TOPICO_PUBLISH8, "1");

  }
  
  //START TIMER
   unsigned long currentMillis = millis(); // Obtém o tempo atual

  // Verifica se o intervalo de 8 horas foi atingido
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Atualiza o tempo anterior

    // Alterna os estados das saídas
    state = !state;
     }
//END TIMER
 
    if ((automatico == 1) && (pressao < 5.00))
      {
         digitalWrite(D5, state ? HIGH : LOW);
         digitalWrite(D10, state ? LOW : HIGH);
      }
  
        if ((automatico == 1) && (pressao > 6.00)) {
            digitalWrite(D5, LOW);
            digitalWrite(D10, LOW);
        }
  

if (automatico == 0) {
  Serial.println("======== SISTEMA EM MANUAL========");
  Serial.println(" ");
    digitalWrite(D5, LOW);
    digitalWrite(D10, LOW);
}
if (emergencia == 0) {
  Serial.println("======= BOTÃO DE EMERGENCIA ACIONADO =======");
  Serial.println(" ");
      digitalWrite(D9, HIGH);
      digitalWrite(D5, LOW);
      digitalWrite(D10, LOW);
        //   MQTT.publish(TOPICO_PUBLISH8, "0");

}
if (RST == 0) {
  Serial.println("=========== FALTA DE FASE ==========");
  Serial.println(" ");
      digitalWrite(D9, HIGH);
        //   MQTT.publish(TOPICO_PUBLISH7, "0");

}
if (central == '0')
  {
   // digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
   // digitalWrite(D7, LOW);
    digitalWrite(D8, LOW);
        //   MQTT.publish(TOPICO_PUBLISH10, "0");

  }
 
/*====================================================================================================
*                             MONITORA E ESCREVE O STATUS DOS COMPRESSORES
======================================================================================================*/

  {
    Serial.println("======= STATUS COMPRESSORES =======");
    Serial.print("compressor1: ");
    Serial.println(statecompressor1);
    Serial.print("compressor2: ");
    Serial.println(statecompressor2);
    Serial.println(" ");
  }

/*================================================================================================
*                        MONITORA E ESCREVE O STATUS DOS SENSORES NO MONITOR
==================================================================================================*/

  {
    Serial.println("======= NOVA LEITURA =======");
    Serial.print("Pressão dos compressores: ");
    Serial.println(leitura);
    Serial.print("Pressão do lado direito: ");
    Serial.println(direito);
    Serial.print("concentração de oxigênio: ");
    Serial.println(oxigenio);
    Serial.println(" ");
  }

   /*=================================================================================================
  *                 LEITURA E ENVIO DOS DADOS DO SENSOR DE PRESSÃO DOS COMPRESSORES
  ====================================================================================================*/

  leitura = (readAnalogAverage(A0) -590);
  pressao = (leitura * 20 / 4095);


  /*=================================================================================================
  *                            LEITURA E ENVIO DOS DADOS DO SENSOR DE OXIGENIO
  ==================================================================================================*/

  oxigenio = (readAnalogAverage(A1) -590);
  concentracao = (oxigenio * 20 / 4095);


  /*=================================================================================================
  *                           LEITURA E ENVIO DOS DADOS DOS SENSORES DO BLOCO CENTRAL
  ====================================================================================================*/

  esquerdo = (readAnalogAverage(A2) - 590);
  cilindroEsq = (esquerdo * 20 / 4095);

/*-----------------------------------------------------------------------------------------------------------------*/
  
  direito = (readAnalogAverage(A3) - 590);
  cilindroDir = (direito * 20 / 4095);
  char TEMP3[5];


  DynamicJsonDocument doc(1024);
  doc['Hospital'] = 'Teste';
  doc['Compressor1'] = statecompressor1;
  doc['conpressorInp'] = compressorINPUT;
  doc['compressor2'] = statecompressor2;
  doc['compressorInp2'] = compressorINPUT2;
  doc['emergencia'] = emergencia;
  doc['Falta de fase'] = RST;
  doc['automatico'] = automatico;
  doc['central'] = central;
  doc['pressao'] = pressao;
  doc['oxigenio'] = concentracao;
  doc['cilindroEsq'] = cilindroEsq;
  doc['cilindroDir'] = cilindroDir;
  String output;
  serializeJson(doc, output);
  output.toCharArray(TEMP3, 8);
  strcpy(payload4, "");
  strcat(payload4, TEMP3);
  strcat(payload4, "");
  strcat(payload4, timestamp4);
  strcat(payload4, "");
  MQTT.publish(TOPICO_12, payload4);


  //garante funcionamento das conexões WiFi e ao broker MQTT
  VerificaConexoesWiFIEMQTT();

  //envia o status de todos os outputs para o Broker no protocolo esperado
  EnviaEstadoOutputMQTT();

  //keep-alive da comunicação com broker MQTT
  MQTT.loop();
}