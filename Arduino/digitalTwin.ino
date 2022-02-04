#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

#include <MPU6050_light.h>
#include "Wire.h"
#include "keys.h"

//WiFi
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

unsigned long previousMillis = 0;

int count = 0;

//angle value
int initialAngle=0;
int currentAngle=0;
int angle = 0;
int deviation = 0;

//Gyroskop
MPU6050 mpu(Wire);
unsigned long timer = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  while(!Serial); //Wait until board is ready

  int wifiStatus = WL_IDLE_STATUS;

  while(wifiStatus!=WL_CONNECTED){
    Serial.print("Connection to ");
    Serial.println(WIFI_SSID);
    wifiStatus = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(2000);
  }

  Serial.print("IP adress: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_BUILTIN, OUTPUT);

  while ( wifiStatus != WL_CONNECTED) {
    wifiStatus = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
  }

  mqttClient.setId(MQTT_CLIENT_NAME);
  mqttClient.setUsernamePassword(MQTT_USER,MQTT_PASSWORD);

  if(!mqttClient.connect(MQTT_BROKER, MQTT_PORT)){
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while(1);
  }

  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
  
}

int oldVal = 0;

void loop() {
 
  //Gyro
  mpu.update();
  
  if((millis()-timer)>10){ // print data every 10ms
	Serial.print("X : ");
	Serial.print(mpu.getAngleX());
	Serial.print("\tY : ");
	Serial.print(mpu.getAngleY());
	Serial.print("\tZ : ");
	Serial.println(mpu.getAngleZ());   
	Serial.print("\tX GYRO : ");
	Serial.println(mpu.getGyroX());    
	Serial.println(" ");  
  }
  //Send Message
  mqttClient.poll();

  //send Message
  mqttClient.beginMessage(MQTT_TOPIC_ANGLE);
  //mqttClient.print(WiFi.RSSI());
  //mqttClient.print(mpu.getAngleZ());// left positive value // right negative value    
  currentAngle = mpu.getAngleZ();
  
  deviation = initialAngle - currentAngle;
  
  if(deviation>20||deviation<-20){
  	if(deviation>0){
		angle = 1;
	}else{
		angle = -1;
	}
  }else{
  	angle = 0;
  }
	
  mqttClient.print(angle);	
  initialAngle = currentAngle;  
	
  mqttClient.endMessage();

  Serial.print("Sending message to topic: ");
  Serial.print(MQTT_TOPIC_ACC);

  mqttClient.beginMessage(MQTT_TOPIC_ACC);
  mqttClient.print(mpu.getGyroX());
  mqttClient.endMessage();

  //delay
  delay(MQTT_INTERVAL);
  
}


