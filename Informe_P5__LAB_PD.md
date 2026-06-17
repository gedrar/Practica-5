# Informe de Pràctica 5: Busos de Comunicació I (I2C)

**Autors:** Julio Lázaro Alcobendas i Gerard Rodríguez González
**Data:** 24 de Maig de 2026
**Repositori GitHub:** https://github.com/gedrar/Practica-5

> **Nota:** El fitxer `platformio.ini` és comú per a tots els exercicis d'aquesta pràctica:
> ```ini
> [env:esp32-s3-devkitc-1]
> platform = espressif32
> board = esp32-s3-devkitc-1
> framework = arduino
> monitor_speed = 115200
> lib_deps =
>     adafruit/Adafruit GFX Library@^1.11.9
>     adafruit/Adafruit SSD1306@^2.5.9
> ```
> Les llibreries d'Adafruit només s'utilitzen a l'Exercici 2 i l'Ampliació. L'Exercici 1 no necessita cap dependència addicional.

---

# Exercici 1: Escàner I2C

## 1. Objectius de la pràctica

L'objectiu d'aquest exercici és comprendre el funcionament del bus I2C i aprendre a detectar els dispositius connectats a ell. Mitjançant un escàner I2C, el programa prova totes les adreces possibles (0x01 a 0x7E) i reporta quines responen, permetent identificar quins perifèrics estan físicament connectats a la placa.

## 2. Desenvolupament i Arquitectura

No es requereix cap muntatge addicional més que els dispositius I2C que es vulguin detectar. El bus I2C de l'ESP32-S3 utilitza per defecte els pins **SDA (GPIO 8)** i **SCL (GPIO 9)**. El programa intenta iniciar una transmissió a cada adreça i comprova si el dispositiu confirma la recepció (ACK).

## 3. Codi Principal (main.cpp)

```cpp
#include <Arduino.h>
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) Serial.println("No I2C devices found\n");
  else               Serial.println("done\n");

  delay(5000); // Espera 5 segons per al proper escaneig
}
```

## 4. Funcionament del codi

El programa recorre les 126 adreces I2C possibles. Per a cada una, inicia una transmissió amb `Wire.beginTransmission()` i comprova el resultat de `Wire.endTransmission()`:

- **Error 0:** El dispositiu ha respost amb ACK → adreça vàlida, es reporta.
- **Error 4:** Error desconegut a l'adreça.
- **Resta de casos:** No hi ha dispositiu a aquella adreça (NACK).

L'escaneig es repeteix cada 5 segons.

## 5. Sortida pel Monitor Sèrie

Amb una pantalla OLED (adreça 0x3C) connectada:

```
I2C Scanner
Scanning...
I2C device found at address 0x3C !
done

Scanning...
I2C device found at address 0x3C !
done
```

Sense cap dispositiu connectat:

```
Scanning...
No I2C devices found
```

## 6. Diagrama de flux

```
Inici Programa
  ↓
Wire.begin() + Serial.begin()
  ↓
Bucle Infinit / loop()
  ↓
Serial.println("Scanning...")
  ↓
Per a address = 1 fins a 126:
  ↓
  Wire.beginTransmission(address)
  ↓
  error = Wire.endTransmission()
  ↓
  error == 0? → Imprimir "Found at 0xXX"
  error == 4? → Imprimir "Unknown error"
  ↓
nDevices == 0? → "No devices found" / "done"
  ↓
delay(5000)
  ↻ (repeteix)
```

## 7. Conclusions

L'escàner I2C és una eina fonamental per verificar el muntatge de hardware. Ens ha permès confirmar l'adreça de la pantalla OLED (0x3C) i comprendre com el protocol I2C utilitza un esquema d'adreces de 7 bits per identificar cada dispositiu al bus.

---

# Exercici 2: Pantalla OLED amb animació

## 1. Objectiu

Controlar una pantalla OLED de 128×64 píxels connectada per I2C (adreça 0x3C) per mostrar una animació: un text desplaçant-se horitzontalment i una cara somrient.

## 2. Desenvolupament

S'utilitzen les llibreries `Adafruit_GFX` i `Adafruit_SSD1306` per dibuixar gràfics sobre la pantalla OLED. El programa defineix una funció auxiliar `drawSmiley()` que dibuixa una cara somrient amb l'ull dret opcional en mode "picant l'ullet". L'animació principal fa desplaçar el text "ENHORABONA" de dreta a esquerra.

## 3. Codi Principal (main.cpp)

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void drawSmiley(bool wink) {
  display.clearDisplay();
  display.drawCircle(64, 32, 30, SSD1306_WHITE);   // Cap
  display.fillCircle(50, 22, 4, SSD1306_WHITE);     // Ull esquerre

  if (wink) {
    display.drawLine(72, 22, 84, 22, SSD1306_WHITE); // Ull dret tancat
  } else {
    display.fillCircle(78, 22, 4, SSD1306_WHITE);    // Ull dret obert
  }

  display.drawLine(50, 46, 78, 46, SSD1306_WHITE);  // Base somriure
  display.drawLine(50, 46, 42, 38, SSD1306_WHITE);  // Comissura esquerra
  display.drawLine(78, 46, 86, 38, SSD1306_WHITE);  // Comissura dreta

  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(3000); // Temps per obrir el monitor sèrie

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Error: No s'ha trobat la pantalla OLED"));
    for(;;);
  }
  Serial.println("Pantalla inicialitzada correctament!");
}

void loop() {
  // Animació text desplaçant-se
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);

  for (int x = SCREEN_WIDTH; x > -180; x -= 4) {
    display.clearDisplay();
    display.setCursor(x, 20);
    display.print("ENHORABONA");
    display.display();
    delay(10);
  }

  // Mostrar cara somrient
  drawSmiley(false);
  delay(1000);
}
```

## 4. Funcionament del codi

Al `setup()`, s'inicialitza la pantalla amb `display.begin()`. Si la pantalla no respon a l'adreça 0x3C, el programa s'atura amb un bucle infinit i reporta l'error pel sèrie.

Al `loop()`, s'executen dues animacions en seqüència: un text de mida gran que es desplaça des del marge dret fins a fora del marge esquerre (decrementant la posició X en 4 píxels cada 10 ms), i una cara somrient estàtica que es manté 1 segon. La funció `display.display()` és obligatòria per enviar el buffer de dibuix a la pantalla via I2C.

## 5. Sortida pel Monitor Sèrie

```
Pantalla inicialitzada correctament! Preparant l'animacio...
```

La pantalla OLED mostra visualment les animacions.

## 6. Diagrama de flux

```
Inici Programa
  ↓
Serial.begin + delay(3000)
  ↓
display.begin(0x3C)
  ↓
Error? → Aturar programa
  ↓
Bucle Infinit / loop()
  ↓
Animació text "ENHORABONA" (x: 128 → -180, pas -4)
  ↓
drawSmiley(false) → mostrar cara
  ↓
delay(1000)
  ↻ (repeteix)
```

## 7. Conclusions

Hem après a usar les primitives gràfiques de la llibreria Adafruit GFX: cercles, línies, text amb mides variables i animació per desplaçament de cursor. La comunicació I2C amb la pantalla és transparent gràcies a la llibreria, però cal recordar sempre cridar `display.display()` per actualitzar la pantalla física.

---

# AMPLIACIÓ DE NOTA: Mini-oscil·loscopi I2C

## 1. Objectiu

Convertir la pantalla OLED en un mini-oscil·loscopi que llegeix un senyal analògic d'un pin GPIO i el visualitza en temps real com una ona en la pantalla.

## 2. Desenvolupament

El programa llegeix contínuament el valor del pin analògic (GPIO 4), l'adapta a les dimensions de la pantalla i l'afegeix a un array circular que representa les mostres visibles. Cada iteració del `loop()`, el buffer es desplaça un píxel a l'esquerra i s'afegeix la nova mostra a la dreta, creant l'efecte d'ona en moviment típic d'un oscil·loscopi.

## 3. Codi Principal (main.cpp)

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int analogPin = 4;
int yValues[SCREEN_WIDTH];

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Error: No s'ha trobat la pantalla OLED"));
    for(;;);
  }

  // Iniciem amb una línia recta al centre
  for (int i = 0; i < SCREEN_WIDTH; i++)
    yValues[i] = SCREEN_HEIGHT / 2;

  display.clearDisplay();
  display.display();
  Serial.println("Mini-oscil·loscopi I2C preparat!");
}

void loop() {
  // Desplacem el buffer cap a l'esquerra
  for (int i = 0; i < SCREEN_WIDTH - 1; i++)
    yValues[i] = yValues[i + 1];

  // Nova mostra del pin analògic
  int raw = analogRead(analogPin);
  yValues[SCREEN_WIDTH - 1] = map(raw, 0, 4095, SCREEN_HEIGHT - 1, 0);

  // Dibuixem l'ona
  display.clearDisplay();
  for (int i = 0; i < SCREEN_WIDTH - 1; i++)
    display.drawLine(i, yValues[i], i + 1, yValues[i + 1], SSD1306_WHITE);

  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE); // Marc
  display.display();

  delay(10); // Controla l'escala de temps
}
```

## 4. Funcionament del codi

L'array `yValues[128]` emmagatzema la posició vertical (en píxels) de cada columna de la pantalla. A cada iteració del `loop()`:

1. El buffer es desplaça una posició a l'esquerra (`yValues[i] = yValues[i+1]`).
2. Es llegeix el valor analògic (0–4095 a l'ESP32-S3) i es mapeja a l'alçada de la pantalla (63–0), invertit per a que voltatges alts apareguin a dalt.
3. Es dibuixa l'ona unint punts consecutius amb `drawLine()`.
4. S'afegeix un rectangle com a marc decoratiu.
5. S'envia el buffer a la pantalla via I2C amb `display.display()`.

## 5. Taula de paràmetres

| Paràmetre | Valor | Descripció |
|-----------|-------|------------|
| `analogPin` | GPIO 4 | Pin d'entrada del senyal |
| Resolució ADC | 12 bits (0–4095) | Resolució de l'ESP32-S3 |
| Amplada pantalla | 128 px | Mostres visibles simultàniament |
| `delay()` | 10 ms | Controla l'escala de temps de l'ona |

## 6. Sortida pel Monitor Sèrie

```
Mini-oscil·loscopi I2C preparat!
```

La pantalla OLED mostra la forma d'ona en temps real.

## 7. Conclusions

Aquest exercici amplia el concepte de pantalla OLED cap a una aplicació pràctica de mesura. Hem après a gestionar un buffer circular per a visualització temporal i a usar `map()` per adaptar rangs de valors. Augmentant el `delay()` s'alenteix l'escala de temps (veure senyals lents); reduint-lo s'accelera (veure senyals ràpids).
