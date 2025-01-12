/*
BinarySensor.cpp

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl

Implementacja klasy BinarySensor
*/

#include "BinarySensor.h"

BinarySensor::BinarySensor(unsigned int pinNumber, unsigned char inMode) {
	if ( (inMode != INPUT) && (inMode != INPUT_PULLDOWN) && (inMode != INPUT_PULLUP) ) {
		connectionError = true;
		return;
	}
	
	connectionError = false;
	this->pinNumber = pinNumber;
	
	pinMode(pinNumber, inMode);
	
	update();
	lastState = read();
}

void BinarySensor::update() {
	lastState = digitalRead(pinNumber);
}

bool BinarySensor::read() {
	return lastState;
}

// Brak pomysłu na sprawdzenie poprawności połączenia
bool BinarySensor::testConnection() {
	return !connectionError;
}
