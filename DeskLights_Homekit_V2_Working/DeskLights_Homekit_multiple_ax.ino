#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

// #define PIN_SWITCH 2

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

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);


void setup() {
	Serial.begin(115200);

	wifi_connect();

  // homekit_storage_reset();

  deskLights = new LedController(&deskStrip, DESK_LED_NUM);
  ceilingLights = new LedController(&ceilingStrip, CEILING_LED_NUM);

	my_homekit_setup();
}

void loop() {

	arduino_homekit_loop();

  deskLights->frame();
  ceilingLights->frame();

	delay(10);
}






//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
//lights
extern "C" homekit_server_config_t config;

extern "C" homekit_characteristic_t cha1_on;
extern "C" homekit_characteristic_t cha1_bright;
extern "C" homekit_characteristic_t cha1_sat;
extern "C" homekit_characteristic_t cha1_hue;

extern "C" homekit_characteristic_t cha2_on;
extern "C" homekit_characteristic_t cha2_bright;
extern "C" homekit_characteristic_t cha2_sat;
extern "C" homekit_characteristic_t cha2_hue;


//switch
extern "C" homekit_characteristic_t cha1_switch_on;
extern "C" homekit_characteristic_t cha2_switch_on;

static uint32_t next_heap_millis = 0;



//desk lights perlin switch
void cha1_switch_on_setter(const homekit_value_t value) {
  Serial.println("Switch 1");

	bool on = value.bool_value;
	cha1_switch_on.value.bool_value = on;	//sync the value

  if(value.bool_value){

    deskLights->setMode(PERLIN);
  }
  else{
    
    deskLights->setMode(SOLID_COLOR);
  }

	LOG_D("Switch: %s", on ? "ON" : "OFF");
}

//ceiling lights perlin switch
void cha2_switch_on_setter(const homekit_value_t value) {
  Serial.println("Switch 2");

	cha1_switch_on.value.bool_value = value.bool_value;	//sync the value
  
  if(value.bool_value){

    ceilingLights->setMode(PERLIN);
  }
  else{
    
    ceilingLights->setMode(SOLID_COLOR);
  }
}

//desk lights on switch
void set1_on(const homekit_value_t v) {
  Serial.println("Light 1");

  Serial.println("state_change");

  cha1_on.value.bool_value = v.bool_value; //sync the value

  if(v.bool_value) {
      deskLights->turnOn();

  } else  {
      deskLights->turnOff();
  }
}

//desk lights hue 
void set1_hue(const homekit_value_t v) {
  Serial.println("Light 1 hue");
  Serial.println("set_hue");

  float hue = v.float_value;
  cha1_hue.value.float_value = hue; //sync the value

  deskLights->setHue(hue);
}

void set1_sat(const homekit_value_t v) {

    Serial.println("set_sat 1");
    float sat = v.float_value;
    cha1_sat.value.float_value = sat; //sync the value

    deskLights->setSat(sat);
}

void set1_bright(const homekit_value_t v) {

    Serial.println("set_bright 1");
    int bright = v.int_value;
    cha1_bright.value.int_value = bright; //sync the value

    deskLights->setBrightness(bright);
}


void set2_on(const homekit_value_t v) {
  Serial.println("Light 2 on");

  Serial.println("state_change");

  cha2_on.value.bool_value = v.bool_value; //sync the value

  if(v.bool_value) {
      ceilingLights->turnOn();

  } else  {
      ceilingLights->turnOff();
  }
}

void set2_hue(const homekit_value_t v) {

  Serial.println("Light 2 hue");
  Serial.println("set_hue");
  Serial.println(v.float_value);

  float hue = v.float_value;
  cha2_hue.value.float_value = hue; //sync the value

  ceilingLights->setHue(hue);
}

void set2_sat(const homekit_value_t v) {

    Serial.println("set_sat 2");
    float sat = v.float_value;
    cha2_sat.value.float_value = sat; //sync the value

    ceilingLights->setSat(sat);
}

void set2_bright(const homekit_value_t v) {

    Serial.println("set_bright 2");
    int bright = v.int_value;
    cha2_bright.value.int_value = bright; //sync the value

    ceilingLights->setBrightness(bright);
}




void my_homekit_setup() {

  //pinMode(PIN_SWITCH, OUTPUT);
	//digitalWrite(PIN_SWITCH, HIGH);

  cha1_on.setter = set1_on;
  cha1_bright.setter = set1_bright;
  cha1_sat.setter = set1_sat;
  cha1_hue.setter = set1_hue;

  cha2_on.setter = set2_on;
  cha2_bright.setter = set2_bright;
  cha2_sat.setter = set2_sat;
  cha2_hue.setter = set2_hue;

  cha1_switch_on.setter = cha1_switch_on_setter;
  cha2_switch_on.setter = cha2_switch_on_setter;
	
	arduino_homekit_setup(&config);
}

void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}

