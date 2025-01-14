/*
BinaryOut.cpp

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl
*/

#include "BinaryOut.h"

BinaryOut::BinaryOut(std::function<void(bool)> writeFunc, std::function<bool(void)> conditionFunc, bool inverted):
	writeFunc(writeFunc),
	conditionFunc(conditionFunc),
	inverted(inverted)
{
	// Wymuś ustawienie stanu na LOW
	currentState = 1;
	write(0);
}

void BinaryOut::write(bool newState) {
	if (currentState == newState) {
		return;
	}
	
	currentState = newState;

	writeFunc( ((inverted) ? !newState : newState) );
}

bool BinaryOut::getState() {
	return currentState;
}

void BinaryOut::update() {
	if (conditionFunc == NULL) {
		return;
	}

	write( conditionFunc() );
}
