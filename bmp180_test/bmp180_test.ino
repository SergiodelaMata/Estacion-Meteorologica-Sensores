
#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
  Serial.println("No se ha encontrado el sensor BMP180");
  while (1) {}
  }
}
  
void loop() {
    
  Serial.print("Presion: ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print("Presion al nivel del mar: ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");
  
  Serial.println();
  delay(500);
}
