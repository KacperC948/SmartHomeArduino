#include <DHT.h>
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>

String isNeedToPowerOnHeater = "";

#define DHTPIN 22
#define MOTION 23
#define WINDOW 24
//#define SOUND 25
#define RAIN A0
#define SOIL A1
#define PUMP_PIN 26
#define LED 27
#define LIGHT 25

DHT dht(DHTPIN, DHT21);
Adafruit_BME280 bme;

String flag;

struct dht21 {
  float temp;
  float hum;
};

struct bme280 { 
  float temperature;
  float pressure;
  float humidity;
};

void setup() {
  Serial.begin(9600);
  dht.begin(); 
  if (!bme.begin(0x76)) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	}
  pinMode(MOTION, INPUT);
  pinMode(WINDOW, INPUT_PULLUP);
  pinMode(SOUND, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LED, INPUT);
}

void loop() {

  readValuesFromSensorsAndSendJsonToNodeMcu();
  flag = Serial.readString();

  if(flag.length() > 1){
    if (flag[0] == '1'){
     Serial.println("Odpalam ogrzewanie"); 
    }
    if (flag[1] == '1'){
      Serial.println("Podlewam kwiatka"); 
    }
  } else {
    Serial.println("Bubel");
  }

}

void readValuesFromSensorsAndSendJsonToNodeMcu(){
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  data["humidity_DHT"] = readDht21().hum;
  data["temperature_DHT"] = readDht21().temp;
  data["humidity_BME"] = readBme280().humidity;
  data["temperature_BME"] = readBme280().temperature;
  data["pressure_BME"] = readBme280().pressure;
  data["isMovementDetected"] = readMotionDetection();
  data["isWindowOpen"] = readWindowDetection();
  data["isRaining"] = readRainDetection();
  data["soil_moisture"] = readSoilMoisture();
  data["isLightOn"] = readLightDetection();

  data.printTo(Serial);
  jsonBuffer.clear();
}

dht21 readDht21() {
  dht21 values;

  values.hum = dht.readHumidity();
  values.temp = dht.readTemperature();

  return values;
}

bme280 readBme280() {
  bme280 values;

  values.temperature = bme.readTemperature();
  values.pressure = bme.readPressure() / 100.0F;
  values.humidity = bme.readHumidity();

  return values;
}

bool readMotionDetection() {

  if(digitalRead(MOTION) == HIGH){
    turnOnLed();
    return true;
  } else {
    turnOffLed();
    return false;
  }
}

bool readWindowDetection(){ 

  if(digitalRead(WINDOW) == HIGH) {
    return true;
  } else {
    return false;
  }
}

bool readRainDetection() {
  int rainValue = analogRead(RAIN);

  if(rainValue < 600){
    return true;
  } else {
    return false;
  }
}

int readSoilMoisture() {
  int soilMoistureValue = analogRead(SOIL);

  if(soilMoistureValue > 400){
    digitalWrite(PUMP_PIN, HIGH);
  } else {
    digitalWrite(PUMP_PIN, LOW);
  }
  
  return soilMoistureValue;
}

bool readLightDetection() {

  if(digitalRead(LIGHT) == HIGH){
    return false;
  } else {
    return true;
  }

}

void turnOnLed() {
  digitalWrite(LED, HIGH);
}

void turnOffLed(){
  digitalWrite(LED, LOW);
}