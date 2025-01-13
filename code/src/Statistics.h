/*
Statistics.h

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl
*/

#pragma once

#include "DataSensor.h"

template <typename DATA>
class Statistics {
public:
	Statistics(DataSensor<DATA> **sensors, unsigned int n); // Wskaźnik do tablicy sensorów i ilość elementów

	DATA max(); // Zwróć największą wartość sensora, metoda nie wywołuje metody update() klasy DataSensor
	DATA min(); // Zwróć najmniejszą wartość sensora,  metoda nie wywołuje metody update() klasy DataSensor
	DATA avg(); // Zwróć wartość średnią ze wszystkich sensorów, metoda nie wywołuje metody update() klasy DataSensor

private:
	DataSensor<DATA> **sensors; // Tablica sensorów
	unsigned int n; // Liczba sensorów w tablicy
};

template <typename DATA>
Statistics<DATA>::Statistics(DataSensor<DATA> **sensors, unsigned int n):
	sensors(sensors),
	n(n)
{}

template <typename DATA>
DATA Statistics<DATA>::max() {
	if ( (sensors == nullptr) || (n == 0) ) {
		return 0;
	}

	DATA ret = sensors[0]->read();

	for (unsigned int i = 1; i < n; i++) {
		if (ret < sensors[i]->read()) {
			ret = sensors[i]->read();
		}
	}

	return ret;
}

template <typename DATA>
DATA Statistics<DATA>::min() {
	if ( (sensors == nullptr) || (n == 0) ) {
		return 0;
	}

	DATA ret = sensors[0]->read();

	for (unsigned int i = 1; i < n; i++) {
		if (ret > sensors[i]->read()) {
			ret = sensors[i]->read();
		}
	}

	return ret;
}

template <typename DATA>
DATA Statistics<DATA>::avg() {
	if ( (sensors == nullptr) || (n == 0) ) {
		return 0;
	}

	DATA ret = 0;

	for (unsigned int i = 0; i < n; i++) {
		ret += sensors[i]->read();
	}

	return ret / n;
}
