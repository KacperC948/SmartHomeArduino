//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//Arduino code

//DHT11 Lib
#include <DHT.h>

//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define DOOR_SENSOR_PIN 24
#define LIGHT_SENSOR_PIN 25
#define MOVEMENT_SENSOR_PIN 23
#define DHTPIN 22

//Initialise Arduino to NodeMCU (19=Rx & 18=Tx)
SoftwareSerial nodemcu(19, 18);
Adafruit_BME280 bme;

String isNeedToPowerOnHeater = "";
int doorState;
//Initialisation of DHT11 Sensor

DHT dht(DHTPIN, DHT21);
float temp;
float hum;
String flaga;

struct DHT21Read {
  float humidity;
  float temperature;
};

struct BME280Read {
  float humidity;
  float temperature;
  float pressure;
};


void setup() {
  Serial.begin(9600);
  pinMode(MOVEMENT_SENSOR_PIN, INPUT);
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  if (!bme.begin(0x76)) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	}
  dht.begin(); 

}

void loop() {
  createData();

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

void createData(){
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  DHT21Read Reading1;
  BME280Read Reading2;
  Reading1 = dht21Sensor();
  Reading2 = bme280Sensor();

  data["humidity_DHT"] = Reading1.humidity;
  data["temperature_DHT"] = Reading1.temperature;
  data["temperature_BME"] = Reading2.temperature;
  data["humidity_BME"] = Reading2.humidity;
  data["pressure_BME"] = Reading2.pressure;
  data["soil_moisture"] = soilSensor();
  data["isDoorOpen"] = doorSensor();
  data["isRaining"] = rainSensor();
  data["isLightOn"] = lightSensor();
  data["isMovementDetected"] = movementSensor();
  data.printTo(Serial);
  jsonBuffer.clear();
}

bool rainSensor(){
    int rainSensor = analogRead(A0); // read state
    if (rainSensor < 600)
    {
      return true;
    }
    else {
      return false;
    }
}

bool lightSensor(){
  int lightState = digitalRead(LIGHT_SENSOR_PIN); // read state

  if (lightState == HIGH) {
    return false; // światło jest wyłączone
  } else {
    return true; // światło jest włączone
  }
}

bool doorSensor(){
    doorState = digitalRead(DOOR_SENSOR_PIN); // read state

  if (doorState == HIGH) {
    return true; //drzwi są otwarte
  } else {
    return false; //drzwi są zamknięte
  }
}

BME280Read bme280Sensor(){
  BME280Read BmeRead;
  BmeRead.temperature = bme.readTemperature();
  BmeRead.pressure = bme.readPressure() / 100.0F;
  BmeRead.humidity = bme.readHumidity();
  return BmeRead;
}

int soilSensor(){
    int rainSensor = analogRead(A1); // read state
    return rainSensor;
}

bool movementSensor(){
  int ruch = digitalRead(MOVEMENT_SENSOR_PIN);      
  if(ruch == HIGH)
  {
   return true; // wykryto ruch
  }
  else  {
    return false; // nie wykryto ruchu
  }
}

DHT21Read dht21Sensor() {
  DHT21Read dhtRead;
  dhtRead.humidity = dht.readHumidity();
  dhtRead.temperature = dht.readTemperature();
  return dhtRead;
}