
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Your Wifi SSID";
char pass[] = "Your Wifi Password";

int sensorPin = A0; 
int sensorValue;  
int limit = 300; 


void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(13, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}


void loop()
{
  Blynk.run();

  sensorValue = analogRead(sensorPin); 
 Serial.println("Analog Value : ");
 Serial.println(sensorValue);

 Blynk.virtualWrite(V5, sensorValue);
 
 if (sensorValue<limit) {
 digitalWrite(13, HIGH); 
 }
 else {
 digitalWrite(13, LOW); 
 }
}
