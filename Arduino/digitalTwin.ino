#include <SPI.h>
#include <WiFiNINA.h>
#include "EspMQTTClient.h"
#include "ArduinoJson.h"
#include "keys.h"

EspMQTTClient client(
  WIFI_SSID,
  WIFI_PASSWORD,
  MQTT_IP,  // MQTT Broker server ip
  MQTT_USER,   // Can be omitted if not needed
  MQTT_PASSWORD,   // Can be omitted if not needed
  MQTT_CLIENT_NAME,     // Client name that uniquely identify your device
  MQTT_PORT              // The MQTT port, default to 1883. this line can be omitted
);

DynamicJsonDocument docIn(1024);
DynamicJsonDocument docOut(1024);

void onConnectionEstablished()
{
  Serial.println("connected to mqtt\n");//lambda payload: payload....
  client.subscribe(MQTT_SUBSCRIBE_CHANNEL, [](const String & payload) {
    Serial.println(payload);
    deserializeJson(docIn, payload);
    const char* key = docIn["key"];
    String keyS = String(key);
    float val = docIn["value"];
    if (keyS=="LED")
    {
      if (val>0.5)
        digitalWrite(D1, HIGH);
      else
        digitalWrite(D1, LOW);
    }
  });
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  while(!Serial); //Wait until board is ready

  int status = WL_IDLE_STATUS;

  while(status!=WL_CONNECTED){
    Serial.print("Connection to ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(2000);
  }

  Serial.print("IP adress: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_BUILTIN, OUTPUT);

  while ( status != WL_CONNECTED) {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
  }
  

}

int oldVal = 0;

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Signal strength in dBm: ");
  Serial.println(WiFi.RSSI());
  delay(1000);

  
  //MQTT Part
  client.loop();

  if (abs(val-oldVal)<10)
    return; //nothing to send
  
  oldVal = val;

  docOut["key"] = "TestMQTTArduino";
  docOut["value"]   = WiFi.RSSI();

  std::string json;
  serializeJson(docOut, json);

  client.publish(MQTT_SUBSCRIBE_CHANNEL, json.c_str());

}

