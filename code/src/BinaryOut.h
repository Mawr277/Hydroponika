/*
BinaryOut.h

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl
*/

#pragma once

#include <functional>

class BinaryOut {
public:
	//************************************************************************************************************//
	// wirteFunc - funkcja realizująca fizyczną zmianę stanu wyjścia											  //
	// conditionFunc - jeżeli nie NULL to dostosuj wyjście do wartości zwracanej przez tą funckje (metoda update) //
	// inverted - fizyczny stan wyjścia będzie odwrotny do tego w oprogramowaniu                                  //      
	//************************************************************************************************************//
	BinaryOut(std::function<void(bool)> writeFunc, std::function<bool(void)> conditionFunc = 0, bool inverted = false);


	void write(bool newState); // Zmień fizyczny stan wyjścia
	bool getState(); // Zwróć obecny stan wyjścia
	void update(); // Dostosuj stan wyjścia do funkcji zwracanej przez consitionFunc

private:
	std::function<bool(void)> conditionFunc;
	std::function<void(bool)> writeFunc;
	bool inverted;
	bool currentState;
};
