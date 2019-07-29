/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App project setup:
    Soil Moisture Value Display widget attached to V4
    Humidity Value Display widget attached to V5
    Temperature Value Display widget attached to V6
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token"; //Blynk Server
//char auth[] = "Your Auth Token";   //Local Server

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Your Wifi SSID";
char pass[] = "2444666668888888";

int sensorPin = A0; 
int sensorValue;  
int limit = 300;

#define DHTPIN 14          // What digital pin we're connected to D5

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

const byte L298N_A_pin = D6;

const byte L298N_A_In1_pin = D1;
const byte L298N_A_In2_pin = D2;

const byte L298N_B_pin = D0;

const byte L298N_B_In3_pin = D3;
const byte L298N_B_In4_pin = D4;

int spd;

/*
 * V0 joystick,  
 */

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void motorSpeed(int prmA, byte prmA1, byte prmA2, int prmB, byte prmB1, byte prmB2)
{
  analogWrite(L298N_A_pin,prmA);
  analogWrite(L298N_B_pin,prmB);
  
  digitalWrite(L298N_A_In1_pin,prmA1);
  digitalWrite(L298N_A_In2_pin,prmA2);
  digitalWrite(L298N_B_In3_pin,prmB1);
  digitalWrite(L298N_B_In4_pin,prmB2);
 
}

BLYNK_WRITE(V1) { //Controlling Speed of Motors
   spd = param.asInt(); // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(V0) {
  int x = param[0].asInt();
  int y = param[1].asInt();

 if ((x==512) && (y==512)) // Rest
 {
   motorSpeed(spd,LOW,LOW,spd,LOW,LOW); 
 }
 else if (y>=950) // Forward 
 {
  motorSpeed(spd,HIGH,LOW,spd,HIGH,LOW); 
  
 }
 else if (y<=100)// Backward
 { 
  motorSpeed(spd,LOW,HIGH,spd,LOW,HIGH); 
  }
 else if (x>=950)// Right
 {  
  motorSpeed(spd,HIGH,LOW,spd,LOW,HIGH); 
  
  }
 else if (x<=100)// Left
 { 
  motorSpeed(spd,LOW,HIGH,spd,HIGH,LOW); 
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(13, OUTPUT);

  pinMode(L298N_A_In1_pin,OUTPUT);
  pinMode(L298N_A_In2_pin,OUTPUT);
  pinMode(L298N_B_In3_pin,OUTPUT);
  pinMode(L298N_B_In4_pin,OUTPUT);

  digitalWrite(L298N_A_In1_pin,LOW);
  digitalWrite(L298N_A_In2_pin,LOW);
  digitalWrite(L298N_B_In3_pin,LOW);
  digitalWrite(L298N_B_In4_pin,LOW);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();

  sensorValue = analogRead(sensorPin); 
  Serial.println("Analog Value : ");
  Serial.println(sensorValue);

  Blynk.virtualWrite(V4, sensorValue);
 
  if (sensorValue<limit) {
   digitalWrite(13, HIGH); 
  }
  else {
   digitalWrite(13, LOW); 
  }
 
  timer.run();
}
