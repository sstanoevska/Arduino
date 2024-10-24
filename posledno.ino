#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <LiquidCrystal.h>

#define DS18B20_1_PIN 32  
#define CSMS_1_PIN 34    
#define CSMS_2_PIN 35     
#define DHT11PIN 33

#define AirValue 3655  
#define WaterValue 1890

DHT dht(DHT11PIN, DHT11);
OneWire oneWire(DS18B20_1_PIN); 
DallasTemperature ds18b20_sensors(&oneWire);  
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

int ds18b20_count = 0;
float lastHumidity = -1; 
int lastSoil1 = -1;
int lastSoil2 = -1; 
String tempReadings="";
String lastTempReadings = "";


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  delay(2000); 
  dht.begin(); 
  ds18b20_count = ds18b20_sensors.getDeviceCount();
  ds18b20_sensors.begin(); 
}

void loop() {
  float temperature = getTemperature();
  float humidity = dht.readHumidity();
  int soil1 = map(analogRead(CSMS_1_PIN), AirValue, WaterValue, 0, 100);
  int soil2 = map(analogRead(CSMS_2_PIN), WaterValue, AirValue, 0, 100);

  if (humidity != lastHumidity || soil1 != lastSoil1 || soil2 != lastSoil2 || lastTempReadings != tempReadings) {
    displayHumiditySoil(temperature, humidity, soil1, soil2);
    lastHumidity = humidity;
    lastSoil1 = soil1;
    lastSoil2 = soil2;
    lastTempReadings = tempReadings; 
  }
  
  delay(2000); 
}

float getTemperature() {
  tempReadings = ""; 
  for (int i = 0; i < ds18b20_count; i++) {
    int temperature = ds18b20_sensors.getTempCByIndex(i);
    tempReadings += "DS18B20_";
    tempReadings += i; 
    tempReadings += ": "; 
    tempReadings += temperature;
  }
  return ds18b20_sensors.getTempCByIndex(0);
}

void displayHumiditySoil(float temperature, float humidity, int soil1, int soil2) {
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("Temperature:");
  lcd.print(tempReadings);
  lcd.print("C"); 
  lcd.print(" Humidity:");
  lcd.print(humidity);
  lcd.print("%");
   lcd.setCursor(0, 1);
  lcd.print("Soil1:");
  lcd.print(soil1);
  lcd.print("% Soil2:");
  lcd.print(soil2);
}

void scrollDisplayLeft() {
  for (int i = 0; i < 16; i++) {
    lcd.scrollDisplayLeft();
    delay(10);
  }
}