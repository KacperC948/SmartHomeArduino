#include <DHT.h>
#include <Adafruit_BME280.h>

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

Adafruit_BME280 bme;

String isNeedToPowerOnHeater = "";

#define DHTPIN 22
#define HCSR501 23
#define DOOR 24
#define SOUND 25
#define RAIN A0
#define PUMP_PIN 26

DHT dht(DHTPIN, DHT21);

float temp;
float hum;

String flaga;

void setup() {
  Serial.begin(9600);
  dht.begin(); 
  if (!bme.begin(0x76)) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	}
  pinMode(HCSR501, INPUT);
  pinMode(DOOR, INPUT_PULLUP);
  pinMode(SOUND, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  motionDetection();
  doorDetection();
  soundDetection();
  rainDetection();
  readHumidity();
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

void motionDetection(){
  int move = digitalRead(HCSR501);

  if(move == HIGH){
    Serial.println("Move detected");
  } else {
    Serial.println("Move is not detected");
  }
}

void doorDetection(){
  int doorIsOpen = digitalRead(DOOR);

  if(doorIsOpen) {
    Serial.println("Door is open");
  } else {
    Serial.println("Door is closed");
  }
}

void soundDetection(){
  int sound = digitalRead(SOUND);

  if(sound == HIGH){
    Serial.println("Sound is not detected");
  } else {
    Serial.println("Sound detected");
  }
}

void rainDetection(){
  int rainValue = analogRead(A0);

  if(rainValue < 1000){
    Serial.println("It's raining");
  } else {
    Serial.println("It's not raining");
  }
}

void readHumidity(){
  int humidityValue = analogRead(A1);
  if(humidityValue > 300){
    digitalWrite(PUMP_PIN, HIGH);
    delay(1500);
  } else {
    digitalWrite(PUMP_PIN, LOW);
  }
  Serial.println(humidityValue);
}