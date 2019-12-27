int sensorValue;
 
void setup()
{
  Serial.begin(9600); // starts the serial port at 9600
}
 
void loop()
{
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
  delay(1000); 
}
