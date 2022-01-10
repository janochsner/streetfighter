#include "EspMQTTClient.h"
#include "ArduinoJson.h"

EspMQTTClient client(
  "maxwlan123",
  "blablub123",
  "maqiatto.com",  // MQTT Broker server ip
  "xfweoqmupsxzzqpyft@sdvgeft.com",   // Can be omitted if not needed
  "blabla",   // Can be omitted if not needed
  "someid1",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

DynamicJsonDocument docIn(1024);
DynamicJsonDocument docOut(1024);

void onConnectionEstablished()
{
  Serial.println("connected to mqtt\n");//lambda payload: payload....
  client.subscribe("xfweoqmupsxzzqpyft@sdvgeft.com/from_twin", [](const String & payload) {
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
  pinMode(A0, INPUT);
  pinMode(D1, OUTPUT); 
  Serial.begin(115200);
  client.enableDebuggingMessages(false);
}

int oldVal = 0;
void loop() {
  client.loop();
  //send sensor val
  int val = analogRead(A0);
  delay(10);
  if (abs(val-oldVal)<10)
    return; //nothing to send
  oldVal = val;
  docOut["key"] = "LightSensor";
  docOut["value"]   = (float)val/1024.0f;
  std::string json;
  serializeJson(docOut, json);
  client.publish("xfweoqmupsxzzqpyft@sdvgeft.com/to_twin", json.c_str());
  delay(50);
 // delay(500);                      // Wait for two seconds (to demonstrate the active low LED)
}