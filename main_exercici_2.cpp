#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Funció auxiliar per dibuixar la cara somrient
void drawSmiley(bool wink) {
  display.clearDisplay(); // Neteja la pantalla

  // Dibuixem el cap (un cercle gran)
  display.drawCircle(64, 32, 30, SSD1306_WHITE); 

  // Ull esquerre (sempre obert)
  display.fillCircle(50, 22, 4, SSD1306_WHITE);

  // Ull dret (obert o picant l'ullet)
  if (wink) {
    // Si pica l'ullet, dibuixem una línia horitzontal
    display.drawLine(72, 22, 84, 22, SSD1306_WHITE); 
  } else {
    // Si està obert, un altre cercle
    display.fillCircle(78, 22, 4, SSD1306_WHITE); 
  }

  // Somriure (fet amb 3 línies)
  display.drawLine(50, 46, 78, 46, SSD1306_WHITE); // Base del somriure
  display.drawLine(50, 46, 42, 38, SSD1306_WHITE); // Comissura esquerra
  display.drawLine(78, 46, 86, 38, SSD1306_WHITE); // Comissura dreta

  display.display(); // Obligatori per mostrar els canvis
}
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Funció auxiliar per dibuixar la cara somrient
void drawSmiley(bool wink) {
  display.clearDisplay(); // Neteja la pantalla

  // Dibuixem el cap (un cercle gran)
  display.drawCircle(64, 32, 30, SSD1306_WHITE); 

  // Ull esquerre (sempre obert)
  display.fillCircle(50, 22, 4, SSD1306_WHITE);

  // Ull dret (obert o picant l'ullet)
  if (wink) {
    // Si pica l'ullet, dibuixem una línia horitzontal
    display.drawLine(72, 22, 84, 22, SSD1306_WHITE); 
  } else {
    // Si està obert, un altre cercle
    display.fillCircle(78, 22, 4, SSD1306_WHITE); 
  }

  // Somriure (fet amb 3 línies)
  display.drawLine(50, 46, 78, 46, SSD1306_WHITE); // Base del somriure
  display.drawLine(50, 46, 42, 38, SSD1306_WHITE); // Comissura esquerra
  display.drawLine(78, 46, 86, 38, SSD1306_WHITE); // Comissura dreta

  display.display(); // Obligatori per mostrar els canvis
}

void setup() {
  Serial.begin(115200);
  
  // ESPEREM 3 SEGONS! Així tens temps d'obrir el monitor sèrie (icona de l'endoll)
  delay(3000); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Error: No s'ha trobat la pantalla OLED"));
    for(;;); 
  }

  Serial.println("Pantalla inicialitzada correctament! Preparant l'animacio..."); 
}

void loop() {
  // --- 1. ANIMACIÓ DEL TEXT "ENHORABONA!" ---
  display.setTextSize(3); // Mida de text enorme (cobreix gairebé mitja pantalla)
  display.setTextColor(SSD1306_WHITE); // Text blanc
  
  display.setTextWrap(false);
  // Fem un bucle des del marge dret (128) fins a fora pel marge esquerre (-180)
  for (int x = SCREEN_WIDTH; x > -180; x -= 4) {
    display.clearDisplay(); // Neteja el fotograma anterior
    display.setCursor(x, 20); // Canviem la posició X a cada pas
    display.print("ENHORABONA");
    display.display(); // Dibuixa el fotograma
    delay(10); // Pausa petita per controlar la velocitat
  }

  // --- 2. ANIMACIÓ DE LA CARA ---
  drawSmiley(false); // Mostrem la cara normal
  delay(1000);       // S'hi està 1 segon
}
void setup() {
  Serial.begin(115200);
  
  // ESPEREM 3 SEGONS! Així tens temps d'obrir el monitor sèrie (icona de l'endoll)
  delay(3000); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Error: No s'ha trobat la pantalla OLED"));
    for(;;); 
  }

  Serial.println("Pantalla inicialitzada correctament! Preparant l'animacio..."); 
}

void loop() {
  // --- 1. ANIMACIÓ DEL TEXT "ENHORABONA!" ---
  display.setTextSize(3); // Mida de text enorme (cobreix gairebé mitja pantalla)
  display.setTextColor(SSD1306_WHITE); // Text blanc
  
  display.setTextWrap(false);
  // Fem un bucle des del marge dret (128) fins a fora pel marge esquerre (-180)
  for (int x = SCREEN_WIDTH; x > -180; x -= 4) {
    display.clearDisplay(); // Neteja el fotograma anterior
    display.setCursor(x, 20); // Canviem la posició X a cada pas
    display.print("ENHORABONA");
    display.display(); // Dibuixa el fotograma
    delay(10); // Pausa petita per controlar la velocitat
  }

  // --- 2. ANIMACIÓ DE LA CARA ---
  drawSmiley(false); // Mostrem la cara normal
  delay(1000);       // S'hi està 1 segon
}