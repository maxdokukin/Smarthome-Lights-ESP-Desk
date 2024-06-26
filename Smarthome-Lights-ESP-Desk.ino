#include <Arduino.h>
#include <arduino_homekit_server.h>

//Wifi
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

//LED
#include <Adafruit_NeoPixel.h>
#define DESK_LED_PIN D1
#define DESK_LED_NUM 61
#define CEILING_LED_PIN D2
#define CEILING_LED_NUM 276

Adafruit_NeoPixel deskStrip = Adafruit_NeoPixel(DESK_LED_NUM, DESK_LED_PIN, NEO_GRB + NEO_KHZ800); 
Adafruit_NeoPixel ceilingStrip = Adafruit_NeoPixel(CEILING_LED_NUM, CEILING_LED_PIN, NEO_GRB + NEO_KHZ800); 

boolean valuesUpdated = false;

#include "LedController.h"
LedController *deskLights;
LedController *ceilingLights;

//Homekit
#include "HomeKit.h"

//Alexa
#include "Alexa.h"

void setup() {

  Serial.begin(115200);

  EEPROM.begin(4096);

	wifi_connect();

  deskLights = new LedController(&deskStrip, DESK_LED_NUM, 1500);
  ceilingLights = new LedController(&ceilingStrip, CEILING_LED_NUM, 1506);

  setupHomeKit();
  setupAlexa();

  syncValuesHomekit();
  syncValuesAlexa();
}

long syncLoopTimer = 0;

void loop() {

	homeKitLoop();
  espalexa.loop();

  deskLights->frame();
  ceilingLights->frame();

  if(valuesUpdated){

    syncValuesHomekit();
    syncValuesAlexa();
    
    valuesUpdated = false;
  }

	delay(10);
}