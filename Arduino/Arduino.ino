#include <DHT.h>

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

String isNeedToPowerOnHeater = "";

#define DHTPIN 22
DHT dht(DHTPIN, DHT21);
float temp;
float hum;
String test = "dupa";
String flaga;

void setup() {
  Serial.begin(9600);
  dht.begin(); 
}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  dht21_func();

  data["humidity"] = hum;
  data["temperature"] = temp;

  data.printTo(Serial);
  jsonBuffer.clear();
  flaga = Serial.readString();

  if(flaga.length() > 1){
    if (flaga[0] == '1'){
     Serial.println("Odpalam ogrzewanie"); 
    }
    if (flaga[1] == '1'){
      Serial.println("Podlewam kwiatka"); 
    }
  } else {
    Serial.println("Bubel");
  }

}

void dht21_func() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();
}