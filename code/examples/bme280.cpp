/*
//	bme280.cpp
//
// 	(C) 2025 Mateusz Bogusławski E:mateusz.boguslawski@ibnet.pl
//  Axel Grewe - angrest@gmx.net 12-Sep-2022 (Author of pocketBME280 lib)
//
// Schemat połączenia:
// BME280 SDA - ESP32 GPIO 16
// BME280 SDC - ESP32 GPIO 17
// BME280 GND - ESP32 GND
// BME280 VCC - ESP32 3,3V
//
*/

#include <pocketBME280.h>
#include "DataSensor.h"

pocketBME280 *bme280;
TwoWire *i2c;
DataSensor<float> *temperature, *humidity;
DataSensor<int> *pressure;

Sensor *sensors[3];

void setup() {
	Serial.begin(115200);

	i2c = new TwoWire(0);
	if (!i2c->begin(16, 17)) {
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

	sensors[0] = temperature = new DataSensor<float>([]()->float{
		bme280->startMeasurement();
		while (bme280->isMeasuring()) {}
		return (float)bme280->getTemperature() / 100.0;
	});

	sensors[1] = humidity = new DataSensor<float>([]()->float{
		bme280->startMeasurement();
		while (bme280->isMeasuring()) {}
		return (float)(bme280->getHumidity() % 1024) * 100.0 / 1024.0;
	});

	sensors[2] = pressure = new DataSensor<int>([]()->int{
		bme280->startMeasurement();
		while (bme280->isMeasuring()) {}
		return (float)bme280->getPressure();
	});
}

void loop() {
	for (Sensor *s : sensors) {
		s->update();
	}

	Serial.printf("Temperature: %.2f `C\n", temperature->read());
	Serial.printf("Humidity: %.2f %c\n", humidity->read(),'%');
	Serial.printf("Pressure: %i Pa\n", pressure->read());

	delay(1000);
}
