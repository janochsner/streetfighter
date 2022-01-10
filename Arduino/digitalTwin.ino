#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "keys.h"


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

unsigned long previousMillis = 0;

int count = 0;

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

  mqttClient.setId(MQTT_CLIENT_NAME);
  mqttClient.setUsernamePassword(MQTT_USER,MQTT_PASSWORD);

  if(!mqttClient.connect(MQTT_BROKER, MQTT_PORT)){
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while(1);
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

  //Send Message
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= MQTT_INTERVAL){
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.print(MQTT_TOPIC);
    Serial.println(WiFi.RSSI());

    //send Message
    /*mqttClient.beginMessage(MQTT_TOPIC);
    mqttClient.print(WiFi.RSSI());
    mqttClient.endMessage();*/

    delay(MQTT_INTERVAL);

  }
  
}

