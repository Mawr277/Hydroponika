/*
waterLevel.cpp

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl

Przykład użycie zwarciowego czujnika poziomu wodu za pomocą klasy BinarySensor

Schemat połączeń
ESP32 GND <-> Sensor zwarciowy <-> ESP32 GPIO 1
*/

#include <Arduino.h>
#include "DataSensor.h"

DataSensor<bool> *waterSensor;

void setup() {
	Serial.begin(11520);
	
	pinMode(1, INPUT_PULLUP);
	waterSensor = new DataSensor<bool>([]()->bool{return digitalRead(1);});
}

void loop() {
	waterSensor->update();
	Serial.println( (waterSensor->read() ? "Jest woda" : "Nie ma wody") );
	delay(100);
}
