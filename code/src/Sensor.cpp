/*
Sensor.cpp

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl

Implementacja klasy Sensor
*/

#include "Sensor.h"

Sensor::Sensor() {}

bool Sensor::connectionErr() {
	return connectionError;
}
