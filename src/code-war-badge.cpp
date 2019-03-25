/*
 * Project code-war-badge
 * Description: Playing Code War on a mesh...
 * Author: James Hagerman
 * Date: 2018/12/12
 */
#include "application.h"
#include "adafruit-led-backpack.h"
#include "neopixel.h"

// Offline mode (for xenons and cloudless):
// SYSTEM_MODE(MANUAL);

// Online mode:
SYSTEM_MODE(AUTOMATIC);

SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

const int maxWord = 5; // don't change this...
char displaybuffer[maxWord] = {' ', ' ', ' ', ' ', '\0'};
const char blank[maxWord] = {' ', ' ', ' ', ' ', '\0'};
// TODO: Move all ui management methods to another file:
void writeWord(const char *word) {
    if (strlcpy(displaybuffer, word, maxWord) >= maxWord) {
        // Name longer than expected
    }
    alpha4.writeDigitAscii(0, displaybuffer[0]);
    alpha4.writeDigitAscii(1, displaybuffer[1]);
    alpha4.writeDigitAscii(2, displaybuffer[2]);
    alpha4.writeDigitAscii(3, displaybuffer[3]);
    alpha4.writeDisplay();
}

bool btnLeft = false;
bool btnMiddleTop = false;
bool btnMiddleBottom = false;
bool btnRightTop = false;
bool btnRightBottom = false;

//==================
// CPU Emulation
// Define a basic process state
typedef struct {
  int id;
  int pc; // program counter (next opcode to execute in memory)
  int state;
  int program; // The actual program to be edited, and then loaded into a random spot in the emulator's memory for war
} PROCESS;

// Set how many processes should be allowed to run ay any given time: 
const int processMax = 10;
// Starting to define a struct that will hold the minimal cpu emulator state
typedef struct {
  bool running;
  // TODO: Figure out how to manage storing and indexing processes in the master list:
  int pid; // next process to execute
  PROCESS processes[processMax]; // List of all known processes
} CPU;

CPU cpu;

void initCPU() {
  cpu.running = false;
  cpu.pid = -1;
}

// Process management functions:
//int addProcess(PROCESS newProcess) {
//  int processSlot = findEmptyProcessSlot();
//  cpu.processess
//}
//
//int removeProcess(int processSlot) {
//  cpu.processess[processSlot] = 
//}

//================
// UI Management
//
// How the UI works
// 
// The UI should be configured so the device always boots into "play" mode. This will allow everyone's default warrior
// program to run out of the box.
//
// A user should be able to use a key sequence to switch to another mode. Holding the middle bottom switch will work
// okay for this on the dev board I've built. When that switch is held it should flash "Pick, Mode, +- ". The user
// should then be able to use the other keys to scroll through the modes. Current mode should be included, non-current
// modes should flash (or maybe use the dots some how for this?). When the middle bottom switch is released, the UI will
// switch to that new mode.
//
// Each mode should have a menu of it's own if need calls for one. So let's just call it a Menu button and go from
// there.
//
// Each mode will also display different things. Perhaps the display will be changed based on settings in the menu?
//
// If nothing else:
//  Play will need to show current health, number of times played, wins, and losses.
//  Run will probably need to show the current state of war it's playing in...
//  View and Edit will need to show address, and opcode at that address. How many addresses? How many opcodes?
//
// Because of the limited UI, menu diving will unfortunately be a thing. Limiting that will be difficult without
// limiting the functionality of the device. Maybe just "Play" and "Edit" modes will be enough.
//
// Play can have running stats,
// Edit can have things like "lock, Unlock", Clear all, maybe "select, copy, paste"?
//
// For opcode reference, we should just use a silk screen...
//
// Configure Modes, their names, and their values:
enum UI_Modes {
  PLAY = 0x0,
  RUN = 0x1,
  VIEW = 0x2,
  EDIT = 0x3
};
const int maxModes = 4; // Should match how many Modes in the Enum above...
const int maxModeNameLen = 5;
const char modeNames[maxModes][maxModeNameLen] = {"Play", "Run ","View","Edit"};

const char* getName(UI_Modes mode) {
  return modeNames[mode];
}

// Struct to hold the current UI state
typedef struct {
  UI_Modes mode;
  int menuVisible;
} UI_STATE;

UI_STATE uiState;

void initUI() {
  uiState.mode = PLAY;
  uiState.menuVisible = false;
}

void showCurrentMode() {
  const char* currentMode = getName(uiState.mode);
  writeWord(currentMode);
  delay(500);
}

// Adafruit_7segment matrix = Adafruit_7segment();
// 
// // IMPORTANT: Set pixel COUNT, PIN and TYPE
// #define PIXEL_PIN D2
// #define PIXEL_COUNT 300
// #define PIXEL_TYPE WS2813
// 
// Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
// 
// void selectExternalMeshAntenna() {
// #if (PLATFORM_ID == PLATFORM_ARGON)
//   digitalWrite(ANTSW1, 1);
//   digitalWrite(ANTSW2, 0);
// #elif (PLATFORM_ID == PLATFORM_BORON)
//   digitalWrite(ANTSW1, 0);
// #else
//   digitalWrite(ANTSW1, 0);
//   digitalWrite(ANTSW2, 1);
// #endif
// }
// 
// 
// //void pingMesh() {
// //  Mesh.publish("mesh-msg", "GW ping");
// //}
// //Timer meshPinger(100, pingMesh); // GWs should ping mesh ever 100ms
// bool meshStatus = false;
// void meshMsgHandler(const char *event, const char *data) {
//   Serial.printlnf("event=%s data=%s", event, data ? data : "NULL");
//   meshStatus = !meshStatus;
//   digitalWrite(D7, meshStatus);
// }



void setup() {
  Serial.begin(9600);

  // Setup network:
  //Mesh.off();

  // Init the hardware:
  pinMode(D7, OUTPUT);

  // Internal pullups means buttons should short to ground:
  pinMode(D5, INPUT_PULLUP); //left
  pinMode(D6, INPUT_PULLUP); //top middle
  pinMode(A3, INPUT_PULLUP); //bottom middle
  pinMode(D2, INPUT_PULLUP); //top right
  pinMode(D9, INPUT_PULLUP); //bottom right

  // Init the screen:
  alpha4.begin(0x70);  // pass in the address

  // Done with hardware, Now init the software:
  initCPU(); // Initialize our CPU Emulation
  initUI(); // Initialize our UI:

  // Show the world we're ready!
  writeWord("Code");
  delay(500);
  writeWord("War ");
  delay(500);
  alpha4.clear();
  alpha4.writeDisplay();
  delay(500);

  // Show our current mode:
  // Note: Mode should only really be shown when it's changed...
  showCurrentMode();

  // Old code:
//   pinMode(D7, OUTPUT);
// 
//   // Setup some GW pinging stuff:
// #if PLATFORM_ID == 12
//   // Argon GW
//   //meshPinger.start();
//   selectExternalMeshAntenna();
// #define MESH_PING_ENABLE
// #elif PLATFORM_ID == 13
//   // Boron
// #elif PLATFORM_ID == 14
//   // Xenon
//   // Also, probably not GW so just listen for messages from the gateway...
//   Mesh.subscribe("mesh-msg", meshMsgHandler);
// #endif
// 
//   // Get matrix going!
//   matrix.begin(0x70);
//   matrix.print(0xBEEF, HEX);
//   matrix.drawColon(true);
//   matrix.writeDisplay();
// 
//   // Neopixel stuff
//   strip.begin();
//   strip.setPixelColor(0, strip.Color(255,0,0));
//   strip.show();
}

void buttonTest() {
  if (btnLeft) {
      writeWord("left");
  } else if (btnMiddleTop) {
      writeWord("mTop");
  } else if (btnMiddleBottom) {
      writeWord("mBtm");
  } else if (btnRightTop) {
      writeWord("rTop");
  } else if (btnRightBottom) {
      writeWord("rBtm");
  } else {
      alpha4.clear();
      alpha4.writeDisplay();
  }
}

void handleUIStateChange() {
  // In theory, when this method is called ever 100ms, we will take all of the existing input, and existing state
  // and determine what state changes have occured. Once we determine a state change has indeeded happened, we will
  // respond accordingly by updating state objects and using those state objects to display new UI messages.
  if (btnMiddleBottom) {
    uiState.menuVisible = true;
  } else {
    uiState.menuVisible = false;
  }
}

void loop() {
//  digitalWrite(D7, HIGH);
//  delay(1000);
//  digitalWrite(D7, LOW);
//  delay(1000);

// #ifdef MESH_PING_ENABLE
//   Mesh.publish("mesh-msg", "GW ping");
//   delay(100);
// #endif
  
  btnLeft = !digitalRead(D5);
  btnMiddleTop = !digitalRead(D6);
  btnMiddleBottom = !digitalRead(A3);
  btnRightTop = !digitalRead(D2);
  btnRightBottom = !digitalRead(D9);

  static uint32_t nextUIUpdate = millis();
  if ((int32_t)(millis() - nextUIUpdate) > 0) {
    nextUIUpdate += 100;
    buttonTest();

    // Not working. Trying to get state management working first:
    handleUIStateChange();
    //displayUI();
  }

}




