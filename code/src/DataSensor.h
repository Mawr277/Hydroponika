/*
DataSensor.h

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl
*/
#pragma once

#include <functional>
#include "Sensor.h"

template <typename DATA>
class DataSensor : public Sensor{
public:
	DataSensor(std::function<DATA(void)> readFunc);

	void update() override; // Odczytaj dane z sensora i nadpisz aktualnie przechowywane wartości
	DATA read();

protected:
	std::function<DATA(void)> readFunction; // Wskaźnik do funckji odczytu
	DATA lastRead; // Ostatnio odczytane dane z sensora
	DATA previousRead;
	bool connectionError; // Czy wystąpił błąd odczytu
};

template <typename DATA>
DataSensor<DATA>::DataSensor(std::function<DATA(void)> readFunc):
	readFunction(readFunc),
	previousRead(0),
	lastRead(0)
{
	update();
}

template <typename DATA>
void DataSensor<DATA>::update() {
	previousRead = lastRead;
	lastRead = readFunction();
}

template <typename DATA>
DATA DataSensor<DATA>::read() {
	return lastRead;
}
