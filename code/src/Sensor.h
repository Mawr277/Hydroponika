/*
Sensor.h

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl
*/
#pragma once

#include <functional>

template <typename DATA>
class Sensor {
public:
	Sensor(std::function<DATA(void)> readFunc);

	void update(); // Odczytaj dane z sensora i nadpisz aktualnie przechowywane wartości
	DATA read();

protected:
	std::function<DATA(void)> readFunction; // Wskaźnik do funckji odczytu
	DATA lastRead; // Ostatnio odczytane dane z sensora
	bool connectionError; // Czy wystąpił błąd odczytu
};

template <typename DATA>
Sensor<DATA>::Sensor(std::function<DATA(void)> readFunc):
	readFunction(readFunc)
{
	update();
}

template <typename DATA>
void Sensor<DATA>::update() {
	lastRead = readFunction();
}

template <typename DATA>
DATA Sensor<DATA>::read() {
	return lastRead;
}
