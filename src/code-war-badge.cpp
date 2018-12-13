/*
 * Project code-war-badge
 * Description: Playing Code War on a mesh...
 * Author: James Hagerman
 * Date: 2018/12/12
 */
#include "application.h"
#include "adafruit-led-backpack.h"
#include "neopixel.h"

//SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

Adafruit_7segment matrix = Adafruit_7segment();

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 300
#define PIXEL_TYPE WS2813

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void selectExternalMeshAntenna() {
#if (PLATFORM_ID == PLATFORM_ARGON)
  digitalWrite(ANTSW1, 1);
  digitalWrite(ANTSW2, 0);
#elif (PLATFORM_ID == PLATFORM_BORON)
  digitalWrite(ANTSW1, 0);
#else
  digitalWrite(ANTSW1, 0);
  digitalWrite(ANTSW2, 1);
#endif
}


//void pingMesh() {
//  Mesh.publish("mesh-msg", "GW ping");
//}
//Timer meshPinger(100, pingMesh); // GWs should ping mesh ever 100ms
bool meshStatus = false;
void meshMsgHandler(const char *event, const char *data) {
  Serial.printlnf("event=%s data=%s", event, data ? data : "NULL");
  meshStatus = !meshStatus;
  digitalWrite(D7, meshStatus);
}

void setup() {
  Serial.begin(9600);

  pinMode(D7, OUTPUT);

  // Setup some GW pinging stuff:
#if PLATFORM_ID == 12
  // Argon GW
  //meshPinger.start();
  selectExternalMeshAntenna();
#define MESH_PING_ENABLE
#elif PLATFORM_ID == 13
  // Boron
#elif PLATFORM_ID == 14
  // Xenon
  // Also, probably not GW so just listen for messages from the gateway...
  Mesh.subscribe("mesh-msg", meshMsgHandler);
#endif

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
//  digitalWrite(D7, HIGH);
//  delay(1000);
//  digitalWrite(D7, LOW);
//  delay(1000);

#ifdef MESH_PING_ENABLE
  Mesh.publish("mesh-msg", "GW ping");
  delay(100);
#endif
}
