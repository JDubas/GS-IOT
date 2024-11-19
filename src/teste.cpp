#include <ESP32Servo.h>
#include <ThingerESP32.h>
#include "DHTesp.h"

#define USERNAME "Dubas"
#define DEVICE_ID "GlobalSolutionHall9000"
#define DEVICE_CREDENTIAL "Hall9000"

#define SSID "Wokwi-GUEST"
#define SSID_PASSWORD ""

bool energiaOn = false;  // Estado do switch (false = off, true = on)
int sliderValue = 0;     // Valor do slider recebido
const int servoPin = 18; // Pino do servo

const int DHT_PIN = 15;

float temp = 0;
int hum = 0;

Servo servo;

DHTesp dhtSensor;
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  // Configuração inicial
  thing.add_wifi(SSID, SSID_PASSWORD);
  Serial.begin(115200);

  thing["temp"] >> outputValue(temp);
  thing["hum"] >> outputValue(hum);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  // Recurso para o controle do switch "Energia"
  thing["Energia"] << [](pson& in) {
    if (in.is_empty()) {
      in = energiaOn; // Retorna o estado atual se solicitado
    } else {
      energiaOn = in; // Atualiza o estado com base no switch
    }
  };

  // Recurso para receber o valor do slider
  thing["Slider"] << inputValue(sliderValue, {
    Serial.print("Valor do slider recebido: ");
    Serial.println(sliderValue);
  });

  // Inicializar o servo
  servo.attach(servoPin, 500, 2400);
  servo.write(0); // Posição inicial (fechado)
  Serial.println("Servo inicializado em posição fechada.");
}

void loop() {
  // Obter dados do sensor de temperatura e umidade
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  hum = data.humidity;
  temp = data.temperature;
  
  // Exibe a umidade e a temperatura no serial
  Serial.print("Umidade: ");
  Serial.println(hum);
  Serial.print("Temperatura: ");
  Serial.println(temp);

  // Verificar as condições para mover o servo
  if (energiaOn && temp >= sliderValue - 2 && temp <= sliderValue + 2 && hum < 50) {
    servo.write(180); // Abre o servo
    Serial.println("Condições atendidas: Servo movido para posição aberta (180°).");
  } else {
    servo.write(0);   // Fecha o servo
    Serial.println("Condições não atendidas: Servo movido para posição fechada (0°).");
  }

  // Processa os eventos do Thinger.io
  thing.handle();

  // Aguardar 5 segundos antes da próxima verificação
  delay(5000);
}
