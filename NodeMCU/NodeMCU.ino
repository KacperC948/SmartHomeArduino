#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "ESP8266WiFi.h"
#include <WiFiClient.h>
#include <HttpClient.h>

//D6 = Rx & D5 = Tx
String isNeedToPowerOnHeater;
String flagi;

const char* ssid = "HomeNet"; // Write here your router's username
const char* password = "j1p2l3p4t5h6s7"; // Write here your router's passward

float test;

struct ReciveFromArduino {
  float humidity_DHT;
  float temperature_DHT;
  float temperature_BME;
  float humidity_BME;
  float pressure_BME;
  int soil_moisture;
  bool isDoorOpen;
  bool isRaining;
  bool isLightOn;
  bool isMovementDetected;
};

void setup() {
  Serial.begin(9600);
  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED) {
  //    delay(500);
  //   //  Serial.print(".");
  // }
  // //print a new line, then print WiFi connected and the IP address
  // Serial.println("");
  // Serial.println("WiFi connected");
  // // Print the IP address
  // Serial.println(WiFi.localIP());

  while (!Serial) continue;


  // rejestracje czujników

}

void loop() {
  // delay(4000);
  // ReciveFromArduino data;
  // data = parseData();
  // Serial.println(data.temperature_DHT);
  // if(data.temperature_DHT < 22){
  //   isNeedToPowerOnHeater = "1";
  //   flagi = isNeedToPowerOnHeater;
  // } else {
  //   isNeedToPowerOnHeater = "0";
  //   flagi = isNeedToPowerOnHeater;
  // }
  //   if(data.humidity_DHT < 60){
  //   isNeedToPowerOnHeater = "1";
  //   flagi += isNeedToPowerOnHeater;
  // } else {
  //   isNeedToPowerOnHeater = "0";
  //   flagi += isNeedToPowerOnHeater;
  // }

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(Serial);

  if (data == JsonObject::invalid()) {
    jsonBuffer.clear();
    return;
  }
  test = data["temperature_DHT"];
  Serial.println(test);

  Serial.println(flagi);
  flagi = "";
}

ReciveFromArduino parseData(){
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(Serial);

  if (data == JsonObject::invalid()) {
    jsonBuffer.clear();
    // return 0;
  }


  ReciveFromArduino dataFromArduino;

  dataFromArduino.humidity_DHT = data["humidity_DHT"];
  dataFromArduino.temperature_DHT = data["temperature_DHT"];
  dataFromArduino.temperature_BME = data["temperature_BME"];
  dataFromArduino.humidity_BME = data["humidity_BME"];
  dataFromArduino.pressure_BME = data["pressure_BME"];
  dataFromArduino.soil_moisture = data["soil_moisture"];
  dataFromArduino.isDoorOpen = data["isDoorOpen"];
  dataFromArduino.isRaining = data["isRaining"];
  dataFromArduino.isLightOn = data["isLightOn"];
  dataFromArduino.isMovementDetected = data["isMovementDetected"];
  // Serial.println(data["temperature_DHT"]);
  // Serial.println(dataFromArduino.temperature_DHT);
  return dataFromArduino;
}

void registerAllSensors(){
  
}

