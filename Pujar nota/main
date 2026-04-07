#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin on connectarem el cable del senyal (Pots fer servir el GPIO 4)
const int analogPin = 4; 

// Array per guardar els valors "Y" de cada píxel de l'ona
int yValues[SCREEN_WIDTH]; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Error: No s'ha trobat la pantalla OLED"));
    for(;;); 
  }

  // Omplim la pantalla inicialment amb una línia recta al mig
  for (int i = 0; i < SCREEN_WIDTH; i++) {
    yValues[i] = SCREEN_HEIGHT / 2;
  }
  
  display.clearDisplay();
  display.display();
  Serial.println("Mini-oscil·loscopi I2C preparat!");
}

void loop() {
  // 1. Desplacem tots els punts de la pantalla cap a l'esquerra (per fer l'efecte de moviment)
  for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
    yValues[i] = yValues[i + 1];
  }

  // 2. Llegim el nou valor del senyal pel pin analògic
  int rawValue = analogRead(analogPin);

  // 3. Adaptem el valor llegit (de 0 a 4095 a l'ESP32) a l'alçada de la pantalla (de 63 a 0)
  // Fixa't que invertim els valors perquè el voltatge més alt pugi cap a dalt de la pantalla
  yValues[SCREEN_WIDTH - 1] = map(rawValue, 0, 4095, SCREEN_HEIGHT - 1, 0);

  // 4. Esborrem el fotograma anterior
  display.clearDisplay();

  // 5. Dibuixem l'ona unint els punts amb línies
  for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
    display.drawLine(i, yValues[i], i + 1, yValues[i + 1], SSD1306_WHITE);
  }

  // Afegim un marc bonic per donar-li l'aspecte d'aparell de laboratori
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

  // Enviem per I2C per actualitzar la pantalla
  display.display();

  // Pausa mínima per donar fluïdesa (pots pujar-la o baixar-la per canviar l'"escala de temps")
  delay(10);
}
