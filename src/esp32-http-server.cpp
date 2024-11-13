#define THINGER_SERIAL_DEBUG
#include <ThingerESP32.h>
#include "DHTesp.h"
#include <Wire.h>
// verificar em qual porta ligou o DHT

#define USERNAME "Dubas"
#define DEVICE_ID "GlobalSolutionHall9000"
#define DEVICE_CREDENTIAL "Hall9000"

#define SSID "Wokwi-GUEST"
#define SSID_PASSWORD ""

int Trig_pin = 18;
int Echo_pin = 5;
long duration;
float Speed_of_sound =0.034;
float dist_in_cm;

#define PIN_VERMELHO 26
#define PIN_AZUL 25
#define PIN_AMARELO 33
const int DHT_PIN = 15;
float temp = 0;
int hum = 0;
float dis = 0;

DHTesp dhtSensor;
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  thing.add_wifi(SSID, SSID_PASSWORD);
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(PIN_VERMELHO, OUTPUT);
  pinMode(PIN_AZUL, OUTPUT);
  pinMode(PIN_AMARELO, OUTPUT);

  pinMode(Trig_pin, OUTPUT);
  pinMode(Echo_pin,INPUT);

    // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  thing["Led Vermelho"] << digitalPin(26);
  thing["Led Azul"] << digitalPin(25);
  thing["Led Amarelo"] << digitalPin(33);

  // resource output example (i.e. reading a sensor value)
  //thing["temp"] >> outputValue("15");
  thing["temp"] >> outputValue(temp);
  thing["hum"] >> outputValue(hum);
  thing["dis"] >> outputValue(dis);

}

void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  hum  = dhtSensor.getHumidity();
  temp = dhtSensor.getTemperature();
  delay (500);
  Serial.print("Umidade: ");  
  Serial.println(data.humidity, 1);
  Serial.print("Temp: ");  
  Serial.println(data.temperature, 0);  
  thing["temp"] >> outputValue(temp);
  thing["hum"] >> outputValue(hum);
 
 
  digitalWrite(Trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_pin,LOW);

  duration =pulseIn(Echo_pin,HIGH);

  dist_in_cm=duration*Speed_of_sound/2;
  Serial.print("Distancia :");
  Serial.println(dist_in_cm);

    thing["dis"] >> outputValue(dist_in_cm);

 Serial.print("Seria assim tão facil? ");  

  delay(2000);  
  thing.handle();
}