#include "HardwareSerial.h"
#include "TinyGPS++.h"

HardwareSerial gpsSerial(2);
TinyGPSPlus gpsPlus;
#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
   //Serial.println(gpsSerial.read());  // read from gps, write to serial debug port
   gpsPlus.encode(gpsSerial.read());
      Serial.print("latitud: ");
      Serial.println(gpsPlus.location.lat());
      Serial.print("longitud: ");
      Serial.println(gpsPlus.location.lng());
   }
  }
}
