#include <OneWire.h>
#include <DallasTemperature.h>

#define rele 3
#define sensor A0
#define leituras 100
int limiar = 500;

unsigned long tempoLigado = 1000;
unsigned long tempoDesligado = 1500;
#define BOIA_INFERIOR 13
#define LDR_PIN A1
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(ONE_WIRE_BUS, INPUT);
  digitalWrite(rele, HIGH);
  sensors.begin();
}

void loop() {
  // -------------------------------
  // LEITURA DA TEMPERATURA
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  
  Serial.print("Temperatura: ");
  if (temperatureC == -127) {
    Serial.println("Erro: sensor não encontrado!");
  } else {
    Serial.print(temperatureC);
    Serial.println(" ºC");
  }

  // -------------------------------
  // LEITURA DA LUZ (MÉDIA DE 10)
  long somaLuz = 0;
  for (int i = 0; i < 10; i++) {
    somaLuz += analogRead(LDR_PIN);
    delay(10);
  }
  int mediaLuz = somaLuz / 10;

  Serial.print("Média LDR: ");
  Serial.println(mediaLuz);

  if (mediaLuz < 450) {
    Serial.println("Está CLARO");
  } else {
    Serial.println("Está ESCURO");
  }

  // -------------------------------
  // LEITURA DA UMIDADE (MÉDIA DE 100)
  long somaLeituras = 0;
  for (int i = 0; i < leituras; i++) {
    somaLeituras += analogRead(sensor);
    delay(50);
  }
  int mediaLeitura = somaLeituras / leituras;

  Serial.print("Leitura média do sensor: ");
  Serial.println(mediaLeitura);

  // -------------------------------
  // MOTOR INTERMITENTE SE UMIDADE BAIXA
  if(mediaLeitura < limiar) {
    digitalWrite(rele, LOW);
    Serial.println("Motor LIGADO (intermitente)");
    delay(tempoLigado);
    
    digitalWrite(rele, HIGH);
    Serial.println("Motor DESLIGADO (intermitente)");
    delay(tempoDesligado);
    }
  // RELEITURA DA UMIDADE
  somaLeituras = 0;
  for (int i = 0; i < leituras; i++) {
    somaLeituras += analogRead(sensor);
    delay(50);
  }
    
    mediaLeitura = somaLeituras / leituras;

    Serial.print("Nova leitura média do sensor: ");
    Serial.println(mediaLeitura);
  

  Serial.println("Umidade suficiente - Motor PARADO.");
  digitalWrite(rele, HIGH);

  delay(100);
}
