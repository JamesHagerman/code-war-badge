/*
 * Project code-war-badge
 * Description: Playing Code War on a mesh...
 * Author: James Hagerman
 * Date: 2018/12/12
 */
#include "application.h"
#include "adafruit-led-backpack.h"
#include "neopixel.h"

SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

Adafruit_7segment matrix = Adafruit_7segment();

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 300
#define PIXEL_TYPE WS2813

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() {
  pinMode(D7, OUTPUT);

  // Get matrix going!
  matrix.begin(0x70);
  matrix.print(0xBEEF, HEX);
  matrix.drawColon(true);
  matrix.writeDisplay();

  // Neopixel stuff
  strip.begin();
  strip.setPixelColor(0, strip.Color(255,0,0));
  strip.show();
}

void loop() {
  digitalWrite(D7, HIGH);
  delay(1000);
  digitalWrite(D7, LOW);
  delay(1000);
}
