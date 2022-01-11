#ifndef KEYS_H
#define KEYS_H

//WIFI
const char* WIFI_SSID     = "FRITZ!Box 7430 HS PH";
const char* WIFI_PASSWORD = "z7xr32vbcnfnux7c73hx";

//MQTT
const char* MQTT_BROKER = "maqiatto.com";
const char* MQTT_CLIENT_NAME = "arduino_mqtt";
const int* MQTT_PORT = 1883; 
const char* MQTT_USER = "jan@jnkeitel.de";
const char* MQTT_PASSWORD = "JQWnSABwr.mC43u";
const char* MQTT_TOPIC_ANGLE = "jan@jnkeitel.de/angle";
const char* MQTT_TOPIC_ACC = "jan@jnkeitel.de/acc";

const long* MQTT_INTERVAL = 500; //Waiting time in milliseconds

#endif
