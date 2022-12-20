#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "ESP8266WiFi.h"
#include <WiFiClient.h>
#include <HttpClient.h>
#include <string> 
#include <iostream>
#include <sstream>  

using namespace std;

const char* ssid = "Dom_2.4G"; // Write here your router's username
const char* password = "#Kosinskiego#4@"; // Write here your router's passward

String isNeedToPowerOnHeater;
String flags;

struct ReciveFromArduino{
  float humidity_DHT;
  float temperature_DHT;
  float humidity_BME;
  float temperature_BME;
  float pressure_BME;
  bool isMovementDetected;
  bool isWindowOpen;
  bool isRaining;
  int soil_moisture;
  bool isLightOn;
};

struct DeviceBme280{
  int id;
  int bme280TempSensorId;
  int bme280HumSensorId;
  int bme280PresSensorId;
};

struct DeviceDht21{
  int id;
  int dht21TempSensorId;
  int dht21HumSensorId;
};

struct DeviceMovementDetector{
  int deviceId;
  int sensorId;
};

struct DeviceWindowOpenDetector{
  int deviceId;
  int sensorId;
};

struct DeviceRainDetector{
  int deviceId;
  int sensorId;
};

struct DeviceSoilMoisture{
  int deviceId;
  int sensorId;
};

struct DeviceLightDetector{
  int deviceId;
  int sensorId;
};

struct Devices{
  DeviceDht21 dht21;
  DeviceBme280 bme280;
  DeviceMovementDetector movementDetector;
  DeviceWindowOpenDetector windowOpenDetector;
  DeviceRainDetector rainDetector;
  DeviceSoilMoisture soilMoisture;
  DeviceLightDetector lightDetector;
};

Devices devices;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (!Serial) continue;

  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }

  Serial.println("WiFi connected");

  devices = registerDevices();
  
  delay(1000);
}

void loop() {
  
  ReciveFromArduino dataFromArduino;

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(Serial); 

  if (data == JsonObject::invalid()) {
    jsonBuffer.clear();
    return;
  }

  dataFromArduino.humidity_DHT = data["humidity_DHT"];
  dataFromArduino.temperature_DHT = data["temperature_DHT"];
  dataFromArduino.humidity_BME = data["humidity_BME"];
  dataFromArduino.temperature_BME = data["temperature_BME"];
  dataFromArduino.pressure_BME = data["pressure_BME"];
  dataFromArduino.isMovementDetected = data["isMovementDetected"];
  dataFromArduino.isWindowOpen = data["isWindowOpen"];
  dataFromArduino.isRaining = data["isRaining"];
  dataFromArduino.soil_moisture = data["soil_moisture"];
  dataFromArduino.isLightOn = data["isLightOn"];

  delay(5000);
  
  postObservation(dataFromArduino.humidity_DHT, devices.dht21.dht21HumSensorId);
  postObservation(dataFromArduino.temperature_DHT, devices.dht21.dht21TempSensorId);
  postObservation(dataFromArduino.humidity_BME, devices.bme280.bme280HumSensorId);
  postObservation(dataFromArduino.temperature_BME, devices.bme280.bme280TempSensorId);
  postObservation(dataFromArduino.pressure_BME, devices.bme280.bme280PresSensorId);
  postObservation(dataFromArduino.isMovementDetected, devices.movementDetector.sensorId);
  postObservation(dataFromArduino.isWindowOpen, devices.windowOpenDetector.sensorId);
  postObservation(dataFromArduino.isRaining, devices.rainDetector.sensorId);
  postObservation(dataFromArduino.isLightOn, devices.lightDetector.sensorId);
  postObservation(dataFromArduino.soil_moisture, devices.soilMoisture.sensorId);

  if(dataFromArduino.temperature_DHT < 22){
    isNeedToPowerOnHeater = "1";
    flags = isNeedToPowerOnHeater;
  } else {
    isNeedToPowerOnHeater = "0";
    flags = isNeedToPowerOnHeater;
  }
    if(dataFromArduino.humidity_DHT < 60){
    isNeedToPowerOnHeater = "1";
    flags += isNeedToPowerOnHeater;
  } else {
    isNeedToPowerOnHeater = "0";
    flags += isNeedToPowerOnHeater;
  }
  //Serial.println(flags);
  flags = "";

}

void postObservation(float value, int sensorId){
  Serial.println("### Float postObersation entered ###");
  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  string postObseravtion = "http://192.168.1.3/" + to_string(sensorId) + "/obseravtion/add";

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& floatValue = JSONbuffer.createObject();
  floatValue["value"] = value;

  char JSONmessageBuffer[100];
  floatValue.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, postObseravtion.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();
    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }

}

void postObservation(bool value, int sensorId){
  Serial.println("### Bool postObersation entered ###");
  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  string postObseravtion = "http://192.168.1.3/" + to_string(sensorId) + "/obseravtion/add";

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& boolValue = JSONbuffer.createObject();
  boolValue["value"] = value;

  char JSONmessageBuffer[100];
  boolValue.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, postObseravtion.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();
    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }
}

void postObservation(int value, int sensorId){
  Serial.println("### Int postObersation entered ###");
  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  string postObseravtion = "http://192.168.1.3/" + to_string(sensorId) + "/obseravtion/add";

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& intValue = JSONbuffer.createObject();
  intValue["value"] = value;

  char JSONmessageBuffer[100];
  intValue.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, postObseravtion.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();
    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }
}

Devices registerDevices(){
  Devices devices;

  devices.dht21 = registerDht21Device();
  devices.bme280 = registerBme280Device();
  devices.movementDetector = registerMovementDetectorDevice();
  devices.windowOpenDetector = registerWindowOpenDetectorDevice();
  devices.rainDetector = registerRainDetectorDevice();
  devices.soilMoisture = registerSoilMoistureDevice();
  devices.lightDetector = registerLightDetectorDevice();
  
  return devices; 
}

DeviceDht21 registerDht21Device(){
  String registerDevice = "http://192.168.1.3:8080/device/register";
  
  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  DeviceDht21 deviceDht21;

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& dht21 = JSONbuffer.createObject();

  dht21["deviceModel"] = "DHT21";
  dht21["MAC"] = WiFi.macAddress();

  JsonArray& dht21Sensors = dht21.createNestedArray("sensors");
  JsonObject& dht21DeviceTempSensor = dht21Sensors.createNestedObject();
  dht21DeviceTempSensor["model"] = "Dht21 temperature sensor";
  dht21DeviceTempSensor["unit"] = "celsius";
  dht21DeviceTempSensor["type"] = "temperature";
  
  JsonObject& dht21DeviceHumSensor = dht21Sensors.createNestedObject();
  dht21DeviceHumSensor["model"] = "Dht21 humidity sensor";
  dht21DeviceHumSensor["unit"] = "percentage";
  dht21DeviceHumSensor["type"] = "humidity";  

  char JSONmessageBuffer[400];
  dht21.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, registerDevice.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();

    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonBuffer dynamicJsonBuffer(capacity);

    JsonObject& device = dynamicJsonBuffer.parseObject(payload.c_str());

    deviceDht21.id = device["id"];
    deviceDht21.dht21TempSensorId = device["sensors"][0]["id"];
    deviceDht21.dht21HumSensorId = device["sensors"][1]["id"];

    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }

  return deviceDht21;  
}

DeviceBme280 registerBme280Device(){
  String registerDevice = "http://192.168.1.3:8080/device/register";

  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  DeviceBme280 deviceBme280;

  StaticJsonBuffer<450> JSONbuffer;

  JsonObject& bme280 = JSONbuffer.createObject();
  
  bme280["deviceModel"] = "BME280";
  bme280["MAC"] = WiFi.macAddress();

  JsonArray& bme280Sensors = bme280.createNestedArray("sensors");
  JsonObject& bme280DeviceTempSensor = bme280Sensors.createNestedObject();
  bme280DeviceTempSensor["model"] = "Bme280 temperature sensor";
  bme280DeviceTempSensor["unit"] = "celsius";
  bme280DeviceTempSensor["type"] = "temperature";
  
  JsonObject& bme280DeviceHumSensor = bme280Sensors.createNestedObject();
  bme280DeviceHumSensor["model"] = "Bme280 humidity sensor";
  bme280DeviceHumSensor["unit"] = "percentage";
  bme280DeviceHumSensor["type"] = "humidity";

  JsonObject& bme280DevicePresSensor = bme280Sensors.createNestedObject();
  bme280DevicePresSensor["model"] = "Bme280 pressure sensor";
  bme280DevicePresSensor["unit"] = "hPa";
  bme280DevicePresSensor["type"] = "atmospheric pressure ";

  char JSONmessageBuffer[450];
  bme280.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, registerDevice.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();

    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(3) + 60;
    DynamicJsonBuffer dynamicJsonBuffer(capacity);

    JsonObject& device = dynamicJsonBuffer.parseObject(payload.c_str());

    deviceBme280.id = device["id"];
    deviceBme280.bme280TempSensorId = device["sensors"][0]["id"];
    deviceBme280.bme280HumSensorId = device["sensors"][1]["id"];
    deviceBme280.bme280PresSensorId = device["sensors"][2]["id"];

    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }

  return deviceBme280;
}

DeviceMovementDetector registerMovementDetectorDevice(){
  String registerDevice = "http://192.168.1.3:8080/device/register";

  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  DeviceMovementDetector deviceMovementDetector;

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& movementDetector = JSONbuffer.createObject();

  movementDetector["deviceModel"] = "Movement detector";
  movementDetector["MAC"] = WiFi.macAddress();

  JsonArray& movementDetectorSensors = movementDetector.createNestedArray("sensors");
  JsonObject& movementDetectorSensor = movementDetectorSensors.createNestedObject();
  movementDetectorSensor["model"] = "Movement detector sensor";
  movementDetectorSensor["unit"] = "boolean";
  movementDetectorSensor["type"] = "logical";

  char JSONmessageBuffer[400];
  movementDetector.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, registerDevice.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();

    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(1) + 60;
    DynamicJsonBuffer dynamicJsonBuffer(capacity);

    JsonObject& device = dynamicJsonBuffer.parseObject(payload.c_str());

    deviceMovementDetector.deviceId = device["id"];
    deviceMovementDetector.sensorId = device["sensors"][0]["id"];

    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }
  
  return deviceMovementDetector;
}

DeviceWindowOpenDetector registerWindowOpenDetectorDevice(){
  String registerDevice = "http://192.168.1.3:8080/device/register";

  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  DeviceWindowOpenDetector deviceWindowOpenDetector;

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& windowOpenDetector = JSONbuffer.createObject();

  windowOpenDetector["deviceModel"] = "Window open detector";
  windowOpenDetector["MAC"] = WiFi.macAddress();

  JsonArray& windowOpenDetectorSensors = windowOpenDetector.createNestedArray("sensors");
  JsonObject& windowOpenDetectorSensor = windowOpenDetectorSensors.createNestedObject();
  windowOpenDetectorSensor["model"] = "Window open detector sensor";
  windowOpenDetectorSensor["unit"] = "boolean";
  windowOpenDetectorSensor["type"] = "logical";

  char JSONmessageBuffer[400];
  windowOpenDetector.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, registerDevice.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();

    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(1) + 60;
    DynamicJsonBuffer dynamicJsonBuffer(capacity);

    JsonObject& device = dynamicJsonBuffer.parseObject(payload.c_str());

    deviceWindowOpenDetector.deviceId = device["id"];
    deviceWindowOpenDetector.sensorId = device["sensors"][0]["id"];

    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }

  return deviceWindowOpenDetector;
}

DeviceRainDetector registerRainDetectorDevice(){
  String registerDevice = "http://192.168.1.3:8080/device/register";

  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  DeviceRainDetector deviceRainDetector;

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& rainDetector = JSONbuffer.createObject();

  rainDetector["deviceModel"] = "Rain detector";
  rainDetector["MAC"] = WiFi.macAddress();

  JsonArray& rainDetectorSensors = rainDetector.createNestedArray("sensors");
  JsonObject& rainDetectorSensor = rainDetectorSensors.createNestedObject();
  rainDetectorSensor["model"] = "Rain detector sensor";
  rainDetectorSensor["unit"] = "boolean";
  rainDetectorSensor["type"] = "logical";

  char JSONmessageBuffer[400];
  rainDetector.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, registerDevice.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();

    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(1) + 60;
    DynamicJsonBuffer dynamicJsonBuffer(capacity);

    JsonObject& device = dynamicJsonBuffer.parseObject(payload.c_str());

    deviceRainDetector.deviceId = device["id"];
    deviceRainDetector.sensorId = device["sensors"][0]["id"];

    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }

  return deviceRainDetector;
}

DeviceSoilMoisture registerSoilMoistureDevice(){
  String registerDevice = "http://192.168.1.3:8080/device/register";

  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  DeviceSoilMoisture deviceSoilMoisture;

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& soilMoistureDetector = JSONbuffer.createObject();

  soilMoistureDetector["deviceModel"] = "Soil moisture detector";
  soilMoistureDetector["MAC"] = WiFi.macAddress();

  JsonArray& soilMoistureDetectorSensors = soilMoistureDetector.createNestedArray("sensors");
  JsonObject& soilMoistureDetectorSensor = soilMoistureDetectorSensors.createNestedObject();
  soilMoistureDetectorSensor["model"] = "Soil moisture detector sensor";
  soilMoistureDetectorSensor["unit"] = "boolean";
  soilMoistureDetectorSensor["type"] = "logical";

  char JSONmessageBuffer[400];
  soilMoistureDetector.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, registerDevice.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();

    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(1) + 60;
    DynamicJsonBuffer dynamicJsonBuffer(capacity);

    JsonObject& device = dynamicJsonBuffer.parseObject(payload.c_str());

    deviceSoilMoisture.deviceId = device["id"];
    deviceSoilMoisture.sensorId = device["sensors"][0]["id"];

    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }

  return deviceSoilMoisture;
}

DeviceLightDetector registerLightDetectorDevice(){
  String registerDevice = "http://192.168.1.3:8080/device/register";

  WiFiClient client;
  HTTPClient http;
  int httpResponse;
  String payload;

  DeviceLightDetector deviceLightDetector;

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& lightDetector = JSONbuffer.createObject();

  lightDetector["deviceModel"] = "Light detector";
  lightDetector["MAC"] = WiFi.macAddress();

  JsonArray& lightDetectorSensors = lightDetector.createNestedArray("sensors");
  JsonObject& lightDetectorSensor = lightDetectorSensors.createNestedObject();
  lightDetectorSensor["model"] = "Light detector sensor";
  lightDetectorSensor["unit"] = "boolean";
  lightDetectorSensor["type"] = "logical";

  char JSONmessageBuffer[400];
  lightDetector.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  if (http.begin(client, registerDevice.c_str())) {
    http.addHeader("Content-Type", "application/json");
    httpResponse = http.POST(JSONmessageBuffer);
    payload = http.getString();

    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(1) + 60;
    DynamicJsonBuffer dynamicJsonBuffer(capacity);

    JsonObject& device = dynamicJsonBuffer.parseObject(payload.c_str());

    deviceLightDetector.deviceId = device["id"];
    deviceLightDetector.sensorId = device["sensors"][0]["id"];

    http.end();
  } else {
      Serial.println("[HTTP] Unable to connect");
  }

  return deviceLightDetector;
}