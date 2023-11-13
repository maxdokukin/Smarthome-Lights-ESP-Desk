#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include "AxillaryFnc.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);


//////PARAMETERS
#define BRIGHTNESS_SMOOTH_TIME 300
#define COLOR_SMOOTH_TIME 400

//////COM VARS
bool received_sat = false;
bool received_hue = false;

//////LED VARS
bool is_on = false;
float current_brightness = 0.0;//0-100
float current_sat = 0.0; //0-100
float current_hue = 0.0; //0-360
int rgb_colors[3];
int prev_rgb_colors[3];

//////TRANSITION VARS
long oldRGB, targetRGB;
float targetBrightness, oldBrightness;
long colChangeTime = -1;
long brChangeTime = -1;


#include "Memory.h"
#include "LED.h"
#include "HomeAlexBridge.h"
#include "Homekit.h"
#include "Alexa.h"

void setup() {
  	
  Serial.begin(115200);
  delay(1000);

  loadFromEEPROM();
  pixelSetup();

	wifi_connect();
  delay(1000);

	my_homekit_setup();
  alexaSetup();
}

void loop() {

	homekitLoop();
  alexaLoop();

  updateTransitions();

	delay(5);
}


