
#include "wifi_info.h"
#include "AxillaryFunctions.h"


#define DEVICE_COUNT 1
#define ALEXA_NAME "Desk Lights"
#define HOMEKIT_NAME "Desk Lights"
#define LED_PIN D2
#define LED_NUM 61
#define BRIGHTNESS_TRANS_TIME 300
#define COLOR_TRANS_TIME 400


///////CLASS DECLARATIONS///////
class LedController;
class MemoryController;
class SmartHomeBridge;
class AlexaController;
class HomekitController;

LedController *ledCon;
MemoryController *memCon;
SmartHomeBridge *bridge;
AlexaController *alxCon;
HomekitController *hmkCon;

#include "LedController.h"
#include "MemoryController.h"
#include "SmartHomeBridge.h"
#include "AlexaController.h"
#include "HomekitController.h"

//divide to heder and cpp files :((()))
////////////////////////////////////////////////////////


Adafruit_NeoPixel deskLed = Adafruit_NeoPixel(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800);;



void setup() {

  Serial.begin(9600);
  wifi_connect();

  ledCon = new LedController(&deskLed);
  memCon = new MemoryController(1500);
  alxCon = new AlexaController("Desk");
  hmkCon = new HomekitController("Desk Lights");
  bridge = new SmartHomeBridge(ledCon, memCon, alxCon, hmkCon);

  memCon->debugOn();
  ledCon->debugOn();

  memCon->loadFromEEPROM(ledCon);
  ledCon->setup();
  alxCon->setup();
  hmkCon->setup();
  bridge->setup();


  memCon->saveByte(1500, 0);
  memCon->saveByte(1501, 255);
  memCon->saveByte(1502, 0);
  memCon->saveByte(1503, 100);
}

void loop() {

  bridge->frame();
  ledCon->frame();
}
