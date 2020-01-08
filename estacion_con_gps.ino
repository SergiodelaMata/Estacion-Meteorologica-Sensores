#include "DHT.h"
#include "Wire.h"
#include "Adafruit_BMP085.h"
#include "Adafruit_VEML6075.h"
#include "HardwareSerial.h"
#include "TinyGPS++.h"
#include "WiFi.h"
#include "HTTPClient.h"

//definimos el DHT11
#define DHTPIN 4   
#define DHTTYPE DHT11   
//inicializamos las variables de los sensores
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
int rainPin = 13;
//varialbes globales para almacenar las mediciones
double temp;
double hum;
double hic;
float bmpV;
float uva;
String rm;
String ldr;
float viento;
String mq;
double latitud;
double longitud;
//nombre y contraseña del wifi al que se conectara la estacion
const char* ssid = "nombreWifi";
const char* password =  "contraseñaWifi";
int id_estacion= 2 ; //id de la estacion
HTTPClient http; 
//url utilizada para el envio de datos
const char* serverName = "http://weatherubicuastation.duckdns.org/addBBDD.php";
const char* serverGPS = "http://weatherubicuastation.duckdns.org/addGPS.php";

//Inicializacion del GPS
HardwareSerial gpsSerial(2);
TinyGPSPlus gpsPlus;
#define RXD2 16
#define TXD2 17


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }  
  bmp.begin();
  dht.begin();
  uv.begin(); 
  pinMode(rainPin, INPUT);
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  //se toman las mediciones y despues se envian
  readGPS();
  readDHT();
  readBMP();
  readRain();
  readLDR();
  readUVA();
  readVeleta();
  readMQ135();
  String httpRequestData = String("?id=") + id_estacion + "&humedad=" + hum  +"&temperatura=" + temp  +"&sensacion_termica=" + hic +"&presion_atmosferica=" + bmpV 
                           + "&cantidad_lluvia=" +rm +"&nivel_luz=" +ldr +"&nivel_radiacion=" +uva +"&angulo_viento=" +viento +"&calidad_aire=" +mq + "&latitud=" +latitud +"&longitud=" +longitud;
  String url = String(serverName) + httpRequestData;
  Serial.println(url);  
  http.begin(url); 
  int httpCode = http.GET();                                    
  Serial.println(httpCode);        
  http.end();
  delay(300000); 

}

void readVeleta(){
   boolean c1,c2,c3,c4;
   c1=false;
   c2=false;
   c3=false;
   c4=false;
   double YValue =analogRead(35);
   double XValue =analogRead(33);
   double y = map(YValue, 776, 4095, -100, 100); // los valores min y max se deben ajustar a cada sensor
   double x = map(XValue, 772, 4095, -100, 100);
   if (y >= 0 && x >= 0){c1=true;} // y+ x+
   if (y < 0 && x >= 0 ){c2=true;} // y- x+
   if (y < 0 && x < 0){c3=true;} // y- x-
   if (y >= 0 && x < 0){c4=true;} // y+ x-
   double formula = 180*(double)(atan((double)(x/y))/PI);
   if (c1){viento = formula;} 
   if (c2 || c3){viento = 180+formula;}
   if (c4){viento = 360+formula;}
}
void readGPS(){
   if (gpsSerial.available()) {
    if (gpsPlus.encode(gpsSerial.read())){
      if(gpsPlus.location.isUpdated()){
        latitud = gpsPlus.location.lat();
        longitud = gpsPlus.location.lng();
        
      }
    }
  }
}

void readMQ135(){
  int sensorValue = analogRead(32);
  if(sensorValue >= 400){
    mq = "Media";
  }
  else if(sensorValue >= 600){
    mq = "Baja";
  } 
  else if(sensorValue >= 800){
    mq = "Pesima";
  }
  else{
    mq = "Buena";
  }
}

void readUVA(){
  uva = uv.readUVI();
}
void readLDR(){
  int sensorValue = analogRead(36);
  if(sensorValue >= 2800){
    ldr = "Soleado";
  }
  else if(sensorValue >= 1200){
    ldr = "Nublado";
  }
  else{
    ldr = "Noche";
  }
}

void readRain(){
    int sensorValue = analogRead(rainPin);
    int range = map(sensorValue, 150, 4095, 0, 2);
  switch (range) {
    case 0:   
      rm = "LLuvia%20intensa";
      break;
    case 1:    
      rm = "LLuvia%20moderada";
      break;
    case 2:    
      rm = "Sin%20lluvia";
      break;
  }
}

void readBMP(){
 bmpV = bmp.readPressure();
}

void readDHT(){
  hum = dht.readHumidity();  
  temp = dht.readTemperature();  
  hic = dht.computeHeatIndex(temp, hum, false);

  

}
