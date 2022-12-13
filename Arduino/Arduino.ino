#include <DHT.h>
#include <Adafruit_BME280.h>

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

Adafruit_BME280 bme;

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
  if (!bme.begin(0x76)) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	}
}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  bme280_func();
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

void bme280_func(){
  Serial.print("Temperature = ");
	Serial.print(bme.readTemperature());
	Serial.println("*C");

	Serial.print("Pressure = ");
	Serial.print(bme.readPressure() / 100.0F);
	Serial.println("hPa");

	Serial.print("Humidity = ");
	Serial.print(bme.readHumidity());
	Serial.println("%");
}

void dht21_func() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();
}