#include <SoftwareSerial.h>
#include <ArduinoJson.h>

String isNeedToPowerOnHeater;
String flagi;

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;
}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(Serial);

  if (data == JsonObject::invalid()) {
    jsonBuffer.clear();
    return;
  }

  float hum = data["humidity"];
  float temp = data["temperature"];

  if(temp < 22){
    isNeedToPowerOnHeater = "1";
    flagi = isNeedToPowerOnHeater;
  } else {
    isNeedToPowerOnHeater = "0";
    flagi = isNeedToPowerOnHeater;
  }
    if(hum < 60){
    isNeedToPowerOnHeater = "1";
    flagi += isNeedToPowerOnHeater;
  } else {
    isNeedToPowerOnHeater = "0";
    flagi += isNeedToPowerOnHeater;
  }
  Serial.println(flagi);
  flagi = "";

}