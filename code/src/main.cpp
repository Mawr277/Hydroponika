#include <Arduino.h>


void setup() {
pinMode(RGB_BUILTIN, OUTPUT);
Serial.begin(115200);
}

void loop() {
delay(100);
digitalWrite(RGB_BUILTIN, HIGH);
delay(100);
digitalWrite(RGB_BUILTIN, LOW);
}