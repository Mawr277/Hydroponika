/*
BinarySensor.h

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl

Plik nagłowkowy klasy BinarySensor
Klasa reprezentuje sensory które mają tylko dwa możliwe stany
(odczyt stanu za pomocą odczytu stanu pina)
*/

#include <Arduino.h>
#include "Sensor.h"

class BinarySensor : public Sensor {
public:
	BinarySensor(unsigned int pinNumber, unsigned char inMode = INPUT);

	void update() override;
	bool testConnection() override;

	bool read();

private:
	bool lastState; // Stan sensora po najnowszym odczycie
	unsigned int pinNumber; // Number pinu połączonego z sensorem
};
