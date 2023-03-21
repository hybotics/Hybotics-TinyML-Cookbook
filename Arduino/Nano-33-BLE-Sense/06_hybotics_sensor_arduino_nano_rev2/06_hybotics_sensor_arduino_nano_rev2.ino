#include <Arduino_HS300x.h>

#define READ_TEMPERATURE() HS300x.readTemperature()
#define READ_HUMIDITY()    HS300x.readHumidity()

float toFahrenheit(float celcius) {
  return  (celcius * 1.8) + 32.0;
}

float toCelsius(float fahrenheit) {
  return (fahrenheit - 32) / 1.8;
}

void setup() {
  Serial.begin(9600);

  while (!Serial);

  if (!HS300x.begin()) {

    Serial.println("Failed initialization of HTS221!");
    while (1);
  }
}

void loop() {
  float celcius, fahrenheit;

  celcius = READ_TEMPERATURE();
  fahrenheit = toFahrenheit(celcius);
  
  Serial.print("Test Temperature = ");
  Serial.print(fahrenheit, 2);
  Serial.print(" °F (");
  Serial.print(celcius, 2);
  Serial.println(" °C)");  
  Serial.print("Test Humidity = ");
  Serial.print(READ_HUMIDITY(), 2);
  Serial.println(" %");
  Serial.println();

  delay(2000);
}
