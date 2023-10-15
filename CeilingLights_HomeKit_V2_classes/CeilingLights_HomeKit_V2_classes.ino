#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel ceilingStrip = Adafruit_NeoPixel(60, D3, NEO_GRB + NEO_KHZ800); 

#include "LedController.h"
LedController *ceilingLights;

void setup() {
	Serial.begin(115200);

	wifi_connect();

  ceilingLights = new LedController(&ceilingStrip, 60);

	my_homekit_setup();
}

void loop() {

	arduino_homekit_loop();

  ceilingLights->frame();

	delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c

extern "C" homekit_server_config_t accessory_config;
extern "C" homekit_characteristic_t cha_on;
extern "C" homekit_characteristic_t cha_bright;
extern "C" homekit_characteristic_t cha_sat;
extern "C" homekit_characteristic_t cha_hue;

void my_homekit_setup() {

  cha_on.setter = set_on;
  cha_bright.setter = set_bright;
  cha_sat.setter = set_sat;
  cha_hue.setter = set_hue;
  
	arduino_homekit_setup(&accessory_config);
}


void set_on(const homekit_value_t v) {

  Serial.println("state_change");

  cha_on.value.bool_value = v.bool_value; //sync the value

  if(v.bool_value) {
      ceilingLights->turnOn();

  } else  {
      ceilingLights->turnOff();
  }
}

void set_hue(const homekit_value_t v) {

    Serial.println("set_hue");
    float hue = v.float_value;
    cha_hue.value.float_value = hue; //sync the value

    ceilingLights->setHue(hue);
}

void set_sat(const homekit_value_t v) {

    Serial.println("set_sat");
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value

    ceilingLights->setSat(sat);
}

void set_bright(const homekit_value_t v) {

    Serial.println("set_bright");
    int bright = v.int_value;
    cha_bright.value.int_value = bright; //sync the value

    ceilingLights->setBrightness(bright);
}

