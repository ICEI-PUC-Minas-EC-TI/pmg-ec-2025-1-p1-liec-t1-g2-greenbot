https://drive.google.com/file/d/1MSk16iaQdw5INK8bNuk5M9tKXE4SSjM9/view?usp=drive_link

/* ========================================================
 Projeto: Vaso Inteligente com Expressões Visuais
 Função: Monitora temperatura, umidade, luminosidade, solo
 e nível de água usando sensores conectados ao ESP32.
 Exibe expressões em um display TFT e envia dados via Bluetooth.
 ========================================================


Integrantes do grupo:
Márcio Mendes Gazzinelli Gomes
André de Oliveira Campolina
Guilherme Santos Pedrosa
Joao Paulo Gomes dos Santos
Josué Procópio Tchykolamuenho da Silva Antônio
Victor Gomes Barreto*/


//URL do github: https://github.com/ICEI-PUC-Minas-EC-TI/pmg-ec-2025-1-p1-liec-t1-g2-greenbot


#include <Adafruit_GFX.h>         // Biblioteca gráfica base
#include <Adafruit_ILI9341.h>     // Controlador do display TFT ILI9341
#include <SPI.h>                  // Comunicação SPI para o display
#include <DHT.h>                  // Biblioteca do sensor DHT
#include <DHT_U.h>                // Suporte adicional ao DHT
#include <BluetoothSerial.h>      // Comunicação Bluetooth com ESP32


// === DEFINIÇÃO DOS PINOS (ajuste conforme montagem) ===
#define TFT_CS    22
#define TFT_DC    5
#define TFT_RST   4
#define PINO_RELE 25
#define SENSOR_SOLO 35
#define PINO_DHT 15
#define DHTTYPE DHT22
#define PINO_LDR 34
#define PINO_BOIA 33


// === OBJETOS GLOBAIS ===
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);  // Objeto para o display TFT
DHT dht(PINO_DHT, DHTTYPE);                                        // Objeto para o sensor DHT22
BluetoothSerial SerialBT;                                          // Objeto para comunicação Bluetooth


// === VARIÁVEIS DE CONFIGURAÇÃO ===
int limiarSoloSeco = 2000;         // Valor limite para considerar o solo seco
unsigned long tempoLigado = 1000;  // Tempo que a bomba ficará ligada
unsigned long tempoDesligado = 3000; // Tempo de descanso após rega
float temperatura = 0;
float umidade = 0;


// =====================================================
// FUNÇÕES DE DESENHO DAS EXPRESSÕES FACIAIS NO DISPLAY
// =====================================================


// Exibe uma legenda na tela com o texto especificado
void desenharTextoLegenda(const char* texto) {
  tft.setCursor(20, 20);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(texto);
}


// Expressão de planta feliz (condições ideais)
void desenharRostoFeliz() {
  tft.fillScreen(ILI9341_DARKGREEN);
  uint16_t cor = ILI9341_WHITE;
  tft.fillRect(60, 80, 20, 20, cor);   // Olho esquerdo
  tft.fillRect(160, 80, 20, 20, cor);  // Olho direito


  // Sorriso (parábola)
  for (int x = 60; x <= 180; x++) {
    int y = -0.01 * (x - 120) * (x - 120) + 180;
    tft.drawPixel(x, y, cor);
    tft.drawPixel(x, y - 1, cor);
    tft.drawPixel(x, y - 2, cor);
  }


  desenharTextoLegenda("Rosto Feliz :)");
}


// Expressão de planta triste (solo seco)
void desenharRostoTriste() {
  tft.fillScreen(ILI9341_RED);
  uint16_t cor = ILI9341_WHITE;
  tft.fillRect(60, 80, 20, 20, cor);
  tft.fillRect(160, 80, 20, 20, cor);


  // Boca triste (parábola invertida)
  for (int x = 60; x <= 180; x++) {
    int y = 0.01 * (x - 120) * (x - 120) + 140;
    tft.drawPixel(x, y, cor);
    tft.drawPixel(x, y + 1, cor);
    tft.drawPixel(x, y + 2, cor);
  }


  desenharTextoLegenda("Rosto Triste :(");
}


// Expressão de sono (ambiente escuro)
void desenharRostoSono() {
  tft.fillScreen(ILI9341_PURPLE);
  uint16_t cor = ILI9341_WHITE;


  // Olhos semicerrados
  for (int x = 0; x <= 20; x++) {
    int y = -0.1 * (x - 10) * (x - 10) + 10;
    tft.drawPixel(60 + x, 90 + y, cor);
    tft.drawPixel(160 + x, 90 + y, cor);
  }


  // Boca reta
  tft.drawFastHLine(80, 180, 80, cor);
  desenharTextoLegenda("Zzzzz...");
}


// Expressão de calor (temperatura alta)
void desenharRostoCalor() {
  tft.fillScreen(ILI9341_BLUE);
  uint16_t cor = ILI9341_WHITE;


  // Olhos abertos com efeito
  for (int i = 0; i < 5; i++) {
    tft.drawLine(60 + i, 80, 80 + i, 100, cor);
    tft.drawLine(80 + i, 80, 60 + i, 100, cor);
    tft.drawLine(160 + i, 80, 180 + i, 100, cor);
    tft.drawLine(180 + i, 80, 160 + i, 100, cor);
  }


  // Boca com língua
  tft.fillRect(100, 160, 40, 10, cor);
  tft.fillCircle(130, 170, 8, cor);
  desenharTextoLegenda("Muito Calor!");
}


// Expressão sem água (boia desativada)
void desenharRostoSemAgua() {
  tft.fillScreen(ILI9341_MAROON);
  uint16_t cor = ILI9341_WHITE;
  tft.drawCircle(70, 90, 10, cor);   // Olhos
  tft.drawCircle(170, 90, 10, cor);

  
  // Boca "x"
  tft.drawLine(80, 170, 160, 170, cor);
  tft.drawLine(80, 170, 120, 190, cor);
  tft.drawLine(160, 170, 120, 190, cor);
  desenharTextoLegenda("Sem Agua!");
}


// =====================================================
// FUNÇÃO SETUP: Executada uma vez no início
// =====================================================
void setup() {
  Serial.begin(9600);                    // Inicializa porta serial
  SerialBT.begin("ESP32_IRRIGACAO");     // Inicializa Bluetooth com nome


  dht.begin();                           // Inicia sensor DHT
  pinMode(PINO_BOIA, INPUT_PULLUP);      // Boia (nível de água)
  pinMode(PINO_RELE, OUTPUT);            // Saída para bomba
  pinMode(SENSOR_SOLO, INPUT);           // Sensor de umidade do solo
  pinMode(PINO_LDR, INPUT);              // Sensor de luz (LDR)
  digitalWrite(PINO_RELE, LOW);          // Bomba inicialmente desligada


  // Inicia o display
  tft.begin();
  tft.setRotation(0);
  tft.setAddrWindow(0, 0, 240, 320);
  desenharRostoFeliz();


  // Leitura inicial da temperatura e umidade (média de 3 leituras)
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


  Serial.println("ESP32 iniciado.");
  SerialBT.println("ESP32 conectado via Bluetooth.");
}


// =====================================================
// FUNÇÃO LOOP: Executada continuamente
// =====================================================
void loop() {
  int estadoBoia = digitalRead(PINO_BOIA);  // Verifica nível de água (boia)


  // Leitura da luminosidade (LDR)
  long somaLuz = 0;
  for (int i = 0; i < 100; i++) {
    somaLuz += analogRead(PINO_LDR);
    delay(5);
  }
  int mediaLuz = somaLuz / 100;


  // Leitura da umidade do solo
  long somaSolo = 0;
  for (int i = 0; i < 100; i++) {
    somaSolo += analogRead(SENSOR_SOLO);
    delay(5);
  }
  int mediaSolo = somaSolo / 100;


  // Envio para monitor serial
  Serial.print("Temp: "); Serial.print(temperatura);
  Serial.print(" | Umidade: "); Serial.print(umidade);
  Serial.print(" | Luz: "); Serial.print(mediaLuz);
  Serial.print(" | Solo: "); Serial.println(mediaSolo);


  // Envio via Bluetooth
  SerialBT.print(temperatura); SerialBT.print(" ,");
  SerialBT.print(umidade);     SerialBT.print(" ,");
  SerialBT.print(mediaLuz);    SerialBT.print(" ,");
  SerialBT.println(mediaSolo);


  // Exibe expressão de acordo com as leituras
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


  // Controle da bomba de irrigação
  if (mediaSolo > limiarSoloSeco) {
    digitalWrite(PINO_RELE, HIGH);    // Liga bomba
    delay(tempoLigado);
    digitalWrite(PINO_RELE, LOW);     // Desliga bomba
    delay(tempoDesligado);
  } else {
    digitalWrite(PINO_RELE, LOW);     // Garante bomba desligada
  }


  delay(1000);  // Espera 1 segundo antes da próxima leitura
}

