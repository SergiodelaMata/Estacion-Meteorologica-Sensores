
int rainPin = 13;
int thresholdValue = 500;

void setup(){
  pinMode(rainPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(rainPin);
  Serial.print(sensorValue );
  if(sensorValue < thresholdValue){
    Serial.println("Con lluvia");
  }
  else {
    Serial.println("Sin lluvia");
  }
  delay(500);
}
