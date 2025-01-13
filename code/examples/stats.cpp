#include <Arduino.h>
#include <DS18B20.h>
#include "DataSensor.h"
#include "Statistics.h"

// Pin connected to DS18B20 DATA
#define DATA_PIN1 5
#define DATA_PIN2 6

DS18B20 *ds1, *ds2;
DataSensor<float> *temperatures[2];
Statistics<float> *tempStats;

void setup(void) {
	Serial.begin(11520);
	
	ds1 = new DS18B20(DATA_PIN1);
	ds2 = new DS18B20(DATA_PIN2);
	
	temperatures[0] = new DataSensor<float>([]()->float{return ds1->getTempC();});
	temperatures[1] = new DataSensor<float>([]()->float{return ds2->getTempC();});

	tempStats = new Statistics<float>(temperatures, 2);
}

void loop(void) { 
	temperatures[0]->update();
	temperatures[1]->update();
	
	Serial.printf("Sensor 1 = %.2f`C\n", temperatures[0]->read());
	Serial.printf("Sensor 2 = %.2f`C\n", temperatures[1]->read());
	Serial.printf("min = %.2f`C\n", tempStats->min());
	Serial.printf("max = %.2f`C\n", tempStats->max());
	Serial.printf("avg = %.2f`C\n", tempStats->avg());
	Serial.println("=========================");


	delay(1000);
}
