#include <Arduino.h>
#include <DS18B20.h>
#include "Sensor.h"

// Pin connected to DS18B20 DATA
#define DATA_PIN 5

DS18B20 *ds;
Sensor<float> *temperature;


void setup(void) {
	Serial.begin(11520);
	
	ds = new DS18B20(5);
	temperature = new Sensor<float>([]()->float{return ds->getTempC();});
}

void loop(void) { 
	temperature->update();
	Serial.printf("%.2f`C\n", temperature->read());

	delay(1000);
}
