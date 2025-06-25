#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>

// === DEFINIÇÃO DOS PINOS ===
#define TFT_CS    22
#define TFT_DC    5
#define TFT_RST   4
#define TFT_SCK   18
#define TFT_MOSI  23
#define TFT_MISO  -1  // Não usado para display

#define PINO_RELE 25
#define SENSOR_SOLO 35
#define PINO_DHT 15
#define DHTTYPE DHT22
#define PINO_LDR 34
#define PINO_BOIA 33

// === OBJETOS E VARIÁVEIS ===
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
DHT dht(PINO_DHT, DHTTYPE);

int limiarSoloSeco = 7000;  // Limite para considerar solo seco
unsigned long tempoLigado = 1000;
unsigned long tempoDesligado = 3000;

// Variáveis globais para temperatura e umidade (lidas uma vez)
float temperatura = 0;
float umidade = 0;

// === EXIBIÇÃO DAS EXPRESSÕES ===
void desenharTextoLegenda(const char* texto) {
  tft.setCursor(20, 20);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(texto);
}

void desenharRostoFeliz() {
  tft.fillScreen(ILI9341_DARKGREEN);
  uint16_t cor = ILI9341_WHITE;
  tft.fillRect(60, 80, 20, 20, cor);
  tft.fillRect(160, 80, 20, 20, cor);
  for (int x = 60; x <= 180; x++) {
    int y = -0.01 * (x - 120) * (x - 120) + 180;
    tft.drawPixel(x, y, cor);
    tft.drawPixel(x, y - 1, cor);
    tft.drawPixel(x, y - 2, cor);
  }
  desenharTextoLegenda("Rosto Feliz :)");
}

void desenharRostoTriste() {
  tft.fillScreen(ILI9341_RED);
  uint16_t cor = ILI9341_WHITE;
  tft.fillRect(60, 80, 20, 20, cor);
  tft.fillRect(160, 80, 20, 20, cor);
  for (int x = 60; x <= 180; x++) {
    int y = 0.01 * (x - 120) * (x - 120) + 140;
    tft.drawPixel(x, y, cor);
    tft.drawPixel(x, y + 1, cor);
    tft.drawPixel(x, y + 2, cor);
  }
  desenharTextoLegenda("Rosto Triste :(");
}

void desenharRostoSono() {
  tft.fillScreen(ILI9341_PURPLE);
  uint16_t cor = ILI9341_WHITE;
  for (int x = 0; x <= 20; x++) {
    int y = -0.1 * (x - 10) * (x - 10) + 10;
    tft.drawPixel(60 + x, 90 + y, cor);
    tft.drawPixel(160 + x, 90 + y, cor);
  }
  tft.drawFastHLine(80, 180, 80, cor);
  desenharTextoLegenda("Zzzzz...");
}

void desenharRostoCalor() {
  tft.fillScreen(ILI9341_BLUE);
  uint16_t cor = ILI9341_WHITE;
  uint16_t corLingua = ILI9341_WHITE;
  for (int i = 0; i < 5; i++) {
    tft.drawLine(60 + i, 80, 80 + i, 100, cor);
    tft.drawLine(80 + i, 80, 60 + i, 100, cor);
    tft.drawLine(160 + i, 80, 180 + i, 100, cor);
    tft.drawLine(180 + i, 80, 160 + i, 100, cor);
  }
  tft.fillRect(100, 160, 40, 10, cor);
  tft.fillCircle(130, 170, 8, corLingua);
  desenharTextoLegenda("Muito Calor!");
}

void desenharRostoSemAgua() {
  tft.fillScreen(ILI9341_MAROON);
  uint16_t cor = ILI9341_WHITE;
  tft.drawCircle(70, 90, 10, cor);
  tft.drawCircle(170, 90, 10, cor);
  tft.drawLine(80, 170, 160, 170, cor);
  tft.drawLine(80, 170, 120, 190, cor);
  tft.drawLine(160, 170, 120, 190, cor);
  desenharTextoLegenda("Sem Agua!");
}

// === SETUP ===
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(PINO_BOIA, INPUT_PULLUP);
  pinMode(PINO_RELE, OUTPUT);
  pinMode(SENSOR_SOLO, INPUT);
  digitalWrite(PINO_RELE, LOW);

  tft.begin();
  tft.setRotation(0);
  tft.setAddrWindow(0, 0, 240, 320);

  // Leitura de temperatura e umidade (somente uma vez)
  float somaTemperatura = 0;
  float somaUmidade = 0;

  for (int i = 0; i < 3; i++) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) somaTemperatura += t;
    if (!isnan(h)) somaUmidade += h;
    delay(1000);
  }

  temperatura = somaTemperatura / 3.0;
  umidade = somaUmidade / 3.0;

  Serial.print("Temperatura média (setup): ");
  Serial.println(temperatura);
  Serial.print("Umidade média (setup): ");
  Serial.println(umidade);

  desenharRostoFeliz();
}

// === LOOP ===
void loop() {
  int estadoBoia = digitalRead(PINO_BOIA);

  // Mostrar a temperatura/umidade fixa
  Serial.print("Temperatura (fixa): ");
  Serial.println(temperatura);
  Serial.print("Umidade (fixa): ");
  Serial.println(umidade);

  // LDR
  long somaLuz = 0;
  for (int i = 0; i < 100; i++) {
    somaLuz += analogRead(PINO_LDR);
    delay(5);
  }
  int mediaLuz = somaLuz / 100;
  Serial.print("Média LDR: ");
  Serial.println(mediaLuz);
  Serial.println(mediaLuz < 500 ? "Está CLARO" : "Está ESCURO");

  // Solo
  long somaSolo = 0;
  for (int i = 0; i < 100; i++) {
    somaSolo += analogRead(SENSOR_SOLO);
    delay(5);
  }
  int mediaSolo = somaSolo / 100;
  Serial.print("Leitura média do sensor de solo: ");
  Serial.println(mediaSolo);

  // Boia
  if (estadoBoia != LOW) {
    Serial.println("Boia ATIVADA: Nível de água presente!");
  } else {
    Serial.println("Boia DESATIVADA: Nível de água baixo!");
  }

  // Rosto no display
  if (estadoBoia == LOW) {
    desenharRostoSemAgua();
  } else if (temperatura >= 30) {
    desenharRostoCalor();
  } else if (mediaLuz >= 500) {
    desenharRostoSono();
  } else if (mediaSolo > limiarSoloSeco) {
    desenharRostoTriste();
  } else {
    desenharRostoFeliz();
  }

  // Controle da bomba
  if (mediaSolo < limiarSoloSeco) {
    Serial.println("Solo seco. Ligando bomba...");
    digitalWrite(PINO_RELE, HIGH);
    delay(tempoLigado);
    digitalWrite(PINO_RELE, LOW);
    Serial.println("Bomba desligada.");
    delay(tempoDesligado);
  } else {
    digitalWrite(PINO_RELE, LOW);
    Serial.println("Solo úmido. Bomba não será ligada.");
  }

  delay(1000);
}
