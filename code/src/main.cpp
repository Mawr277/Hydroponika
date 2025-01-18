/*
	main.cpp
*/

#include <Arduino.h>
#include <pocketBME280.h>
#include <DS18B20.h>

#include "BinaryOut.h"
#include "DataSensor.h"

// Piny sterujące przekaźnikami
#define RELAY_DONICZKA_1 4
#define RELAY_DONICZKA_2 5

// Piny połączone z czujnikiem poziomu wody
#define WL_DONICZKA_1 7
#define WL_DONICZKA_2 15
#define WL_ZBIORNIK 16

// Piny połączone z pinami DATA w DS18B20
#define DS18B20_1 8
#define DS18B20_2 9

// Połączenia z BME280
#define BME280_SDA 21
#define BME280_SCL 47

#define SENSORS_NUM 8

pocketBME280 *bme280;
TwoWire *i2c;
DS18B20 *ds18b20[2];

BinaryOut *relays[2];
DataSensor<bool> *waterLevels[3];
DataSensor<float> *temperatures[3];
DataSensor<int> *pressure;
DataSensor<float> *humidity;

Sensor *sensors[SENSORS_NUM];

void setup() {
	Serial.begin(115200);

	// Konfiguracja pinów połączonych z czujnikami poziomu cieczy
	pinMode(WL_DONICZKA_1, INPUT_PULLUP);
	pinMode(WL_DONICZKA_2, INPUT_PULLUP);
	pinMode(WL_ZBIORNIK, INPUT_PULLUP);
	sensors[0] = waterLevels[0] = new DataSensor<bool>([]()->bool{return digitalRead(WL_DONICZKA_1);});
	sensors[1] = waterLevels[1] = new DataSensor<bool>([]()->bool{return digitalRead(WL_DONICZKA_2);});
	sensors[2] = waterLevels[2] = new DataSensor<bool>([]()->bool{return digitalRead(WL_ZBIORNIK);});
	
	// Konfiguracja pinów połączonych z przekaźnikami
	pinMode(RELAY_DONICZKA_1, OUTPUT);
	pinMode(RELAY_DONICZKA_2, OUTPUT);

	// Konfiguracja działania przekaźników
	relays[0] = new BinaryOut(
		// writeFunc
		[](bool newState){ digitalWrite(RELAY_DONICZKA_1, newState); },

		// conditionFunc
		[](){ return !waterLevels[0]->read(); },

		// inverted
		true
	);
	
	relays[1] = new BinaryOut(
		// writeFunc
		[](bool newState){ digitalWrite(RELAY_DONICZKA_2, newState); },

		// conditionFunc
		[](){ return !waterLevels[1]->read(); },

		// inverted
		true
	);
	
	// Konfiguracja DS18B20
	ds18b20[0] = new DS18B20(DS18B20_1);
	ds18b20[1] = new DS18B20(DS18B20_2);
	
	sensors[3] = temperatures[0] = new DataSensor<float>([]()->float{return ds18b20[0]->getTempC();});
	sensors[4] = temperatures[1] = new DataSensor<float>([]()->float{return ds18b20[1]->getTempC();});

	// Konfiguracja BME280
	i2c = new TwoWire(0);
	if (!i2c->begin(BME280_SDA, BME280_SCL)) {
		while (1) {
			Serial.println("Failed to initialize i2c!");
			delay(100);
		}
	}

	bme280 = new pocketBME280();
	if (!bme280->begin(*i2c)) {
		while (1) {
			Serial.println("Failed to initialize bme280 sensor!");
			delay(100);
		}
	}

	sensors[5] = temperatures[2] = new DataSensor<float>([]()->float{
		bme280->startMeasurement();
		while (bme280->isMeasuring()) {} // Measurement in progress
		return (float)bme280->getTemperature() / 100.0;
	});

	sensors[6] = humidity = new DataSensor<float>([]()->float{
		bme280->startMeasurement();
		while (bme280->isMeasuring()) {} // Measurement in progress
		return (float)bme280->getHumidity() / 1024.0;
	});

	sensors[7] = pressure = new DataSensor<int>([]()->int{
		bme280->startMeasurement();
		while (bme280->isMeasuring()) {} // Measurement in progress
		return (float)bme280->getPressure();
	});
}

void loop() {
	// Update sensorów
	Serial.println("Przed");
	for (int i = 0; i < SENSORS_NUM; i++) {
		sensors[i]->update();
	}
	Serial.println("Po");

	relays[0]->update();
	relays[1]->update();

	Serial.printf("%.2f`C\n", temperatures[0]->read());
	Serial.printf("%.2f`C\n", temperatures[1]->read());
	Serial.printf("%.2f`C\n", temperatures[2]->read());

	Serial.printf("%.2f%c\n", humidity->read(), '%');

	Serial.printf("%dPa\n", pressure->read());

	for (int i = 0; i < 3; i++) {
		Serial.printf("Sensor wody %d: ", i+1);
		Serial.println( (waterLevels[i]->read() ? "Jest woda" : "Nie ma wody") );
	}

	Serial.print("Pompka 0: ");
	Serial.println(relays[0]->getState());
	Serial.print("Pompka 1: ");
	Serial.println(relays[1]->getState());

	Serial.println("============================");

	delay(100);
}
