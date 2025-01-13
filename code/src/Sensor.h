/*
Sensor.h

(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl
*/

#pragma once

class Sensor {
public:
	Sensor(){};
	virtual void update() = 0;
};
