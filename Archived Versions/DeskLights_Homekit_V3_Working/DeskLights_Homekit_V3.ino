#include <Arduino.h>
#include <arduino_homekit_server.h>

//Wifi
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

//LED
#define LED_CONRTROLLER_DEBUG true

#include <Adafruit_NeoPixel.h>
#define DESK_LED_PIN D1
#define DESK_LED_NUM 61
#define CEILING_LED_PIN D3
#define CEILING_LED_NUM 151

Adafruit_NeoPixel deskStrip = Adafruit_NeoPixel(DESK_LED_NUM, DESK_LED_PIN, NEO_GRB + NEO_KHZ800); 
Adafruit_NeoPixel ceilingStrip = Adafruit_NeoPixel(CEILING_LED_NUM, CEILING_LED_PIN, NEO_GRB + NEO_KHZ800); 

#include "LedController.h"
LedController *deskLights;
LedController *ceilingLights;

//Homekit
#include "HomeKit.h"

void setup() {

  EEPROM.begin(4096);

  deskLights = new LedController(&deskStrip, DESK_LED_NUM, 1500);
  ceilingLights = new LedController(&ceilingStrip, CEILING_LED_NUM, 1506);

  Serial.begin(9600);

	wifi_connect();

  setupHomeKit();
  homeKitsyncAllValues();
}

void loop() {

	homeKitLoop();

  deskLights->frame();
  ceilingLights->frame();

	delay(10);
}



