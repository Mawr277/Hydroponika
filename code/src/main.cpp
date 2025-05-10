/*
	main.cpp
*/

#include <Arduino.h>
#include <pocketBME280.h>
#include <DS18B20.h>
#include "Adafruit_ILI9341.h"

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

// Piny połączone z wyswietlaczem
#define TFT_MOSI 11
#define TFT_MISO 13
#define TFT_SCK 12
#define TFT_CS 3
#define TFT_RST 46
#define TFT_DC 10

// Połączenia z BME280
#define BME280_SDA 21
#define BME280_SCL 47

#define SENSORS_NUM 5

pocketBME280 *bme280;
TwoWire *i2c;
DS18B20 *ds18b20[2];
Adafruit_ILI9341 *display;

BinaryOut *relays[2];
DataSensor<bool> *waterLevels[2];
DataSensor<bool> *mainWaterTank;
DataSensor<float> *temperatures[3];
DataSensor<int> *pressure;
DataSensor<float> *humidity;

Sensor *sensors[SENSORS_NUM];

String float2string (float value, int precision = 1);

enum class ProgramState {
	ReadSensors,
	FillUpWater,
	NoWater
};
ProgramState currentState;

void setup() {
	currentState = ProgramState::ReadSensors;

	Serial.begin(115200);

	// Inicjalizacja wyswietlacza
	display = new Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
	display->begin(1200000000);
	display->setTextSize(2);
	display->fillScreen(ILI9341_BLACK);

	// Konfiguracja pinów połączonych z czujnikami poziomu cieczy
	pinMode(WL_DONICZKA_1, INPUT_PULLUP);
	pinMode(WL_DONICZKA_2, INPUT_PULLUP);
	pinMode(WL_ZBIORNIK, INPUT_PULLUP);
	waterLevels[0] = new DataSensor<bool>([]()->bool{return digitalRead(WL_DONICZKA_1);});
	waterLevels[1] = new DataSensor<bool>([]()->bool{return digitalRead(WL_DONICZKA_2);});
	mainWaterTank = new DataSensor<bool>([]()->bool{return digitalRead(WL_ZBIORNIK);});
	
	// Konfiguracja pinów połączonych z przekaźnikami
	pinMode(RELAY_DONICZKA_1, OUTPUT);
	pinMode(RELAY_DONICZKA_2, OUTPUT);

	// Konfiguracja działania przekaźników
	relays[0] = new BinaryOut(
		// writeFunc
		[](bool newState){ digitalWrite(RELAY_DONICZKA_1, newState); },

		// conditionFunc
		[](){
			waterLevels[0]->update(); 
			return !waterLevels[0]->read(); 
		},

		// inverted
		true
	);
	
	relays[1] = new BinaryOut(
		// writeFunc
		[](bool newState){ digitalWrite(RELAY_DONICZKA_2, newState); },

		// conditionFunc
		[](){
			waterLevels[1]->update(); 
			return !waterLevels[1]->read(); 
		},

		// inverted
		true
	);
	
	// Konfiguracja DS18B20
	ds18b20[0] = new DS18B20(DS18B20_1);
	ds18b20[1] = new DS18B20(DS18B20_2);
	
	sensors[0] = temperatures[0] = new DataSensor<float>([]()->float{return ds18b20[0]->getTempC();});
	sensors[1] = temperatures[1] = new DataSensor<float>([]()->float{return ds18b20[1]->getTempC();});

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

	sensors[2] = temperatures[2] = new DataSensor<float>([]()->float{
		bme280->startMeasurement();
		while (bme280->isMeasuring()) {} // Measurement in progress
		return (float)bme280->getTemperature() / 100.0;
	});

	sensors[3] = humidity = new DataSensor<float>([]()->float{
		bme280->startMeasurement();
		while (bme280->isMeasuring()) {} // Measurement in progress
		return (float)bme280->getHumidity() / 1024.0;
	});

	sensors[4] = pressure = new DataSensor<int>([]()->int{
		bme280->startMeasurement();
		while (bme280->isMeasuring()) {} // Measurement in progress
		return (float)bme280->getPressure();
	});
}

void loop() {

	// Sprawdz czy nie brakuje wody w glownym zbiorniku
	mainWaterTank->update();
	if (mainWaterTank->read() == LOW) {
		if (currentState == ProgramState::FillUpWater) {
			// Wylacz pompy
			relays[0]->write(LOW);
			relays[1]->write(LOW);
		}

		if (currentState != ProgramState::NoWater) {
			currentState = ProgramState::NoWater;

			display->fillScreen(ILI9341_BLACK);
			display->setCursor(0, 0);
			display->setTextColor(ILI9341_WHITE);  
			display->println("Hydroponika");
			display->println("");
			display->setTextColor(ILI9341_RED);  
			display->println("Uzupelnij wode!");
			display->setTextColor(ILI9341_WHITE); 
		}

		return;
	}

	// Sprawdz czy nie trzeba uzupelnic wody
	relays[0]->update();
	relays[1]->update();
	if ( ( relays[0]->getState() | relays[1]->getState() ) == HIGH) {
		if (currentState != ProgramState::FillUpWater) {
			currentState = ProgramState::FillUpWater;
			display->fillScreen(ILI9341_BLACK);
			display->setCursor(0, 0);
			display->setTextColor(ILI9341_WHITE);  
			display->println("Hydroponika");
			display->println("");
			display->setTextColor(ILI9341_BLUE);  
			display->println("Uzupełnianie wody");
			display->setTextColor(ILI9341_WHITE);  
		}
	} else if (currentState != ProgramState::ReadSensors) {
		currentState = ProgramState::ReadSensors;
	}

	if (currentState == ProgramState::ReadSensors) {
		// Update sensorów
		for (int i = 0; i < SENSORS_NUM; i++) {
			sensors[i]->update();
		}

		display->fillScreen(ILI9341_BLACK);
		display->setCursor(0, 0);
		display->setTextColor(ILI9341_WHITE);  
		display->println("Hydroponika");
		display->println("");
		display->println((float2string(temperatures[0]->read()) + " `C").c_str());
		display->println((float2string(temperatures[1]->read()) + " `C").c_str());
		display->println((float2string(temperatures[0]->read()) + " `C").c_str());
		display->println((float2string(humidity->read()) + " %").c_str());
		display->println((std::to_string(pressure->read()) + " Pa").c_str());
	}
}


String float2string (float value, int precision) {
    char buffer[16];
    dtostrf(value, 0, precision, buffer);
    return String(buffer);
}
