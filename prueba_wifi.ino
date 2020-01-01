#include "DHT.h"
#include "Wire.h"
#include "Adafruit_BMP085.h"
#include "Adafruit_VEML6075.h"
#include "HardwareSerial.h"
#include "TinyGPS++.h"
#include "WiFi.h"
#include "HTTPClient.h"

#define DHTPIN 4   
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
int rainPin = 13;
//nombre y contraseña de vuestro wifi
const char* ssid = "";
const char* password =  "";
HTTPClient http;

const char* serverName = "http://weatherubicuastation.duckdns.org/addBBDD.php";

//GPS, puede que este mal
HardwareSerial gpsSerial(2);
TinyGPSPlus gpsPlus;
#define RXD2 16
#define TXD2 17
//fin GPS

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }  
  //bmp.begin();
  dht.begin();
  //uv.begin(); 
  pinMode(rainPin, INPUT);
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  //readGPS();
  float dht[3];
  readDHT(&dht[0]);
  //readBMP();
  //readRain();
  //readLDR();
  //readUVA();
  //readMQ135();
  //cambiad el id a otro distinto  
  String httpRequestData = String("&id=") + 1 + "&humedad=" + dht[0]  +"&temperatura=" + dht[1]  +"&sensacionTermica=" +dht[2];
  String url = String(serverName) + httpRequestData;
  Serial.println(url);
  
  http.begin(url); 
  int httpCode = http.GET();                                    
  //Serial.println(httpCode);
        
  http.end();
  delay(10000);
  

}

void readGPS(){
   if (gpsSerial.available()) {
   //Serial.println(gpsSerial.read());  // devuelve el NMEA
    if (gpsPlus.encode(gpsSerial.read())){
      Serial.print("latitud: ");
      Serial.println(gpsPlus.location.lat());
      Serial.print("longitud: ");
      Serial.println(gpsPlus.location.lng());
   }
  }
}

void readMQ135(){
  int adc_MQ = analogRead(32); 
  float voltaje = adc_MQ * (5.0 / 1023.0); 

  Serial.print("adc:");
  Serial.print(adc_MQ);
  Serial.print("    voltaje:");
  Serial.println(voltaje);
}

void readUVA(){
  Serial.print("Indice UV: "); Serial.println(uv.readUVI());
  
}
void readLDR(){
  int sensorValue;
  sensorValue = analogRead(36); 
  Serial.print(sensorValue);
  if(sensorValue >= 2700){
    Serial.println(" Soleado");
  }
  else if(sensorValue >= 1000){
    Serial.println(" Nublado");
  }
  else{
    Serial.println(" Noche");
  }
}

void readRain(){
  int sensorValue = analogRead(rainPin);
  Serial.print(sensorValue );
  if(sensorValue < 500){
    Serial.println("Con lluvia");
  }
  else {
    Serial.println("Sin lluvia");
  }
}

void readBMP(){
  Serial.print("Presion: ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print("Presion al nivel del mar: ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");
}

void readDHT(float *array){
  array[0]=dht.readHumidity();  
  array[1]=dht.readTemperature();  
  array[2]=dht.computeHeatIndex(t, h, false);

  

}
