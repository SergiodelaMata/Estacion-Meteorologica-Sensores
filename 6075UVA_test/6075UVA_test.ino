#include <Wire.h>
#include "Adafruit_VEML6075.h"

Adafruit_VEML6075 uv = Adafruit_VEML6075();

void setup() {
  Serial.begin(9600);
  
  if (! uv.begin()) {
    Serial.println("Failed to communicate with VEML6075 sensor, check wiring?");
    while (1) { delay(100); }
  }
}


void loop() {
  Serial.print("UV Index reading: "); Serial.println(uv.readUVI());
  delay(1000);
}
