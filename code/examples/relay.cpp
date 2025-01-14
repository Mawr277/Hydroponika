/*
	relay.cpp

	(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl

	Schemat połączeń:
	Moduł przekaźników = X
	X VCC <-> Zasilacz +5V
	X GND <-> Zasilacz GND
	X GND <-> ESP32 GND
	X IN1 <-> ESP32 GPIO 4
	ESP32 GPIO 5 <-> Czujnik zwarciowy <-> ESP32 GND

	Oczekiwane zachowanie:
	Przy zmianie stanu czujnika (zwiera albo nie) nastąpi
	zmiana stanu przekaźnika
*/

#include <Arduino.h>

#include "BinaryOut.h"
#include "DataSensor.h"

#define RELAY_PIN 4
#define WATER_PIN 5

BinaryOut *relay;
DataSensor<bool> *waterLevel;


void setup() {
	pinMode(RELAY_PIN, OUTPUT);
	pinMode(WATER_PIN, INPUT_PULLUP);

	waterLevel = new DataSensor<bool>([]()->bool{ return digitalRead(WATER_PIN); });

	relay = new BinaryOut(
		// writeFunc
		[](bool newState){ digitalWrite(RELAY_PIN, newState); },

		// conditionFunc
		[](){ return !waterLevel->read(); },

		// inverted
		true
	);
}

void loop() {
	waterLevel->update();
	relay->update();

	delay(100);
}
