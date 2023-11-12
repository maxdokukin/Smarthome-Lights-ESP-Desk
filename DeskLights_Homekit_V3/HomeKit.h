// HomeKit Configuration and Loop

// HomeKit server configuration
extern "C" homekit_server_config_t config;
static uint32_t next_heap_millis = 0;

// Desk Lights Characteristics
extern "C" homekit_characteristic_t desk_on, desk_bright, desk_sat, desk_hue, desk_switch_on;

// Ceiling Lights Characteristics
extern "C" homekit_characteristic_t ceiling_on, ceiling_bright, ceiling_sat, ceiling_hue, ceiling_switch_on;

// Function Declarations
void homeKitsyncAllValues();
void setupHomeKit();
void homeKitLoop();

// Desk Lights Functions

void desk_switch_on_setter(const homekit_value_t value) {
  Serial.println("Switch 1");
  desk_switch_on.value.bool_value = value.bool_value;
  deskLights->setMode(value.bool_value ? PERLIN : SOLID_COLOR);
}

void set1_on(const homekit_value_t value) {
  Serial.println("Light 1 ON/OFF");
  desk_on.value.bool_value = value.bool_value;
  value.bool_value ? deskLights->turnOn() : deskLights->turnOff();
}

void set1_hue(const homekit_value_t value) {
  Serial.println("Light 1 Hue");
  desk_hue.value.float_value = value.float_value;
  deskLights->setHue(value.float_value);
}

void set1_sat(const homekit_value_t value) {
  Serial.println("Light 1 Saturation");
  desk_sat.value.float_value = value.float_value;
  deskLights->setSat(value.float_value);
}

void set1_bright(const homekit_value_t value) {
  Serial.println("Light 1 Brightness");
  desk_bright.value.int_value = value.int_value;
  deskLights->setBrightness(value.int_value);
}

// Ceiling Lights Functions

void ceiling_switch_on_setter(const homekit_value_t value) {
  Serial.println("Switch 2");
  ceiling_switch_on.value.bool_value = value.bool_value;
  ceilingLights->setMode(value.bool_value ? PERLIN : SOLID_COLOR);
}

void set2_on(const homekit_value_t value) {
  Serial.println("Light 2 ON/OFF");
  ceiling_on.value.bool_value = value.bool_value;
  value.bool_value ? ceilingLights->turnOn() : ceilingLights->turnOff();
}

void set2_hue(const homekit_value_t value) {
  Serial.println("Light 2 Hue");
  ceiling_hue.value.float_value = value.float_value;
  ceilingLights->setHue(value.float_value);
}

void set2_sat(const homekit_value_t value) {
  Serial.println("Light 2 Saturation");
  ceiling_sat.value.float_value = value.float_value;
  ceilingLights->setSat(value.float_value);
}

void set2_bright(const homekit_value_t value) {
  Serial.println("Light 2 Brightness");
  ceiling_bright.value.int_value = value.int_value;
  ceilingLights->setBrightness(value.int_value);
}

// Synchronize all values
void homeKitsyncAllValues() {
  desk_on.value.bool_value = deskLights->getState();
  desk_bright.value.int_value = deskLights->getBrightness();
  desk_sat.value.float_value = deskLights->getSat();
  desk_hue.value.float_value = deskLights->getHue();
  desk_switch_on.value.bool_value = deskLights->getMode();

  ceiling_on.value.bool_value = ceilingLights->getState();
  ceiling_bright.value.int_value = ceilingLights->getBrightness();
  ceiling_sat.value.float_value = ceilingLights->getSat();
  ceiling_hue.value.float_value = ceilingLights->getHue();
  ceiling_switch_on.value.bool_value = ceilingLights->getMode();
}

// HomeKit Setup
void setupHomeKit() {
  desk_on.setter = set1_on;
  desk_bright.setter = set1_bright;
  desk_sat.setter = set1_sat;
  desk_hue.setter = set1_hue;
  desk_switch_on.setter = desk_switch_on_setter;

  ceiling_on.setter = set2_on;
  ceiling_bright.setter = set2_bright;
  ceiling_sat.setter = set2_sat;
  ceiling_hue.setter = set2_hue;
  ceiling_switch_on.setter = ceiling_switch_on_setter;

  arduino_homekit_setup(&config);
}

// HomeKit Loop
void homeKitLoop() {
  arduino_homekit_loop();
  uint32_t currentTime = millis();
  if (currentTime > next_heap_millis) {
    next_heap_millis = currentTime + 5000;
    LOG_D("Free heap: %d, HomeKit clients: %d", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
  }
}
