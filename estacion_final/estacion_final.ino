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
const char* ssid = "MOVISTAR_EC4E";
const char* password =  "925818941wifi";
int id_estacion= 2 ;
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
  float dht[3];
  float bmp[1];
  float uva[1];
  String rm[1];
  String ldr[1];
  float viento[1];
  String mq[1];
  //readGPS();
  readDHT(&dht[0]);
  readBMP(&bmp[0]);
  readRain(&rm[0]);
  readLDR(&ldr[0]);
  readUVA(&uva[0]);
  readVeleta(&viento[0]);
  readMQ135(&mq[0]);
  String httpRequestData = String("?id=") + id_estacion + "&humedad=" + dht[0]  +"&temperatura=" + dht[1]  +"&sensacion_termica=" +dht[2] +"&presion_atmosferica=" +bmp[0] 
                           +"&cantidad_lluvia=" +rm[0] +"&nivel_luz=" +ldr[0] +"&nivel_radiacion=" +uva[0] +"&angulo_viento=" +viento[2] +"&calidad_aire=" +mq[0];
  String url = String(serverName) + httpRequestData;
  Serial.println(url);  
  http.begin(url); 
  int httpCode = http.GET();                                    
  Serial.println(httpCode);        
  http.end();
  delay(10000); 

}

void readVeleta(float *array){
   boolean c1,c2,c3,c4;
   c1=false;
   c2=false;
   c3=false;
   c4=false;
   double YValue =analogRead(32);
   double XValue =analogRead(33);
   double y = map(YValue, 776, 4095, -100, 100); // los valores min y max se deben ajustar a cada sensor
   double x = map(XValue, 772, 4095, -100, 100);
   array[0] = x;
   array[1] = y;
   if (y >= 0 && x >= 0){c1=true;} // y+ x+
   if (y < 0 && x >= 0 ){c2=true;} // y- x+
   if (y < 0 && x < 0){c3=true;} // y- x-
   if (y >= 0 && x < 0){c4=true;} // y+ x-
   double formula = 180*(double)(atan((double)(x/y))/PI);
   if (c1){array[2] = formula;} 
   if (c2 || c3){array[2] = 180+formula;}
   if (c4){array[2] = 360+formula;}
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

void readMQ135(String *array){
  int sensorValue = analogRead(32);
  if(sensorValue >= 400){
    array[0] = "Media";
  }
  else if(sensorValue >= 600){
    array[0] = "Baja";
  } 
  else if(sensorValue >= 800){
    array[0] = "Pesima";
  }
  else{
    array[0] = "Buena";
  }
}

void readUVA(float *array){
  array[0] = uv.readUVI();
}
void readLDR(String *array){
  int sensorValue = analogRead(36);
  if(sensorValue >= 2800){
    array[0] = "Soleado";
  }
  else if(sensorValue >= 1200){
    array[0] = "Nublado";
  }
  else{
    array[0] = "Noche";
  }
}

void readRain(String *array){
    int sensorValue = analogRead(rainPin);
    int range = map(sensorValue, 150, 4095, 0, 3);
  switch (range) {
    case 0:   
      array[0] = "LLuvia intensa";
      break;
    case 1:    
      array[0] = "LLuvia moderada";
      break;
    case 2:    
      array[0] = "Sin lluvia";
      break;
  }
}

void readBMP(float *array){
  array [0] = bmp.readPressure();
}

void readDHT(float *array){
  
  float h=dht.readHumidity();  
  array[0]=h;
  
  float t=dht.readTemperature();  
  array[1]=t;
  array[2]=dht.computeHeatIndex(t, h, false);

  

}
