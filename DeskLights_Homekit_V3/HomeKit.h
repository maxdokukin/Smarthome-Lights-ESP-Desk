// HomeKit Configuration and Loop

// HomeKit server configuration
extern "C" homekit_server_config_t config;
static uint32_t next_heap_millis = 0;

// Desk Lights Characteristics
extern "C" homekit_characteristic_t cha1_on, cha1_bright, cha1_sat, cha1_hue, cha1_switch_on;

// Ceiling Lights Characteristics
extern "C" homekit_characteristic_t cha2_on, cha2_bright, cha2_sat, cha2_hue, cha2_switch_on;

// Function Declarations
void homeKitsyncAllValues();
void setupHomeKit();
void homeKitLoop();

// Desk Lights Functions

void cha1_switch_on_setter(const homekit_value_t value) {
  Serial.println("Switch 1");
  cha1_switch_on.value.bool_value = value.bool_value;
  deskLights->setMode(value.bool_value ? PERLIN : SOLID_COLOR);
}

void set1_on(const homekit_value_t value) {
  Serial.println("Light 1 ON/OFF");
  cha1_on.value.bool_value = value.bool_value;
  value.bool_value ? deskLights->turnOn() : deskLights->turnOff();
}

void set1_hue(const homekit_value_t value) {
  Serial.println("Light 1 Hue");
  cha1_hue.value.float_value = value.float_value;
  deskLights->setHue(value.float_value);
}

void set1_sat(const homekit_value_t value) {
  Serial.println("Light 1 Saturation");
  cha1_sat.value.float_value = value.float_value;
  deskLights->setSat(value.float_value);
}

void set1_bright(const homekit_value_t value) {
  Serial.println("Light 1 Brightness");
  cha1_bright.value.int_value = value.int_value;
  deskLights->setBrightness(value.int_value);
}

// Ceiling Lights Functions

void cha2_switch_on_setter(const homekit_value_t value) {
  Serial.println("Switch 2");
  cha2_switch_on.value.bool_value = value.bool_value;
  ceilingLights->setMode(value.bool_value ? PERLIN : SOLID_COLOR);
}

void set2_on(const homekit_value_t value) {
  Serial.println("Light 2 ON/OFF");
  cha2_on.value.bool_value = value.bool_value;
  value.bool_value ? ceilingLights->turnOn() : ceilingLights->turnOff();
}

void set2_hue(const homekit_value_t value) {
  Serial.println("Light 2 Hue");
  cha2_hue.value.float_value = value.float_value;
  ceilingLights->setHue(value.float_value);
}

void set2_sat(const homekit_value_t value) {
  Serial.println("Light 2 Saturation");
  cha2_sat.value.float_value = value.float_value;
  ceilingLights->setSat(value.float_value);
}

void set2_bright(const homekit_value_t value) {
  Serial.println("Light 2 Brightness");
  cha2_bright.value.int_value = value.int_value;
  ceilingLights->setBrightness(value.int_value);
}

// Synchronize all values
void homeKitsyncAllValues() {
  cha1_on.value.bool_value = deskLights->getState();
  cha1_bright.value.int_value = deskLights->getBrightness();
  cha1_sat.value.float_value = deskLights->getSat();
  cha1_hue.value.float_value = deskLights->getHue();
  cha1_switch_on.value.bool_value = deskLights->getMode();

  cha2_on.value.bool_value = ceilingLights->getState();
  cha2_bright.value.int_value = ceilingLights->getBrightness();
  cha2_sat.value.float_value = ceilingLights->getSat();
  cha2_hue.value.float_value = ceilingLights->getHue();
  cha2_switch_on.value.bool_value = ceilingLights->getMode();
}

// HomeKit Setup
void setupHomeKit() {
  cha1_on.setter = set1_on;
  cha1_bright.setter = set1_bright;
  cha1_sat.setter = set1_sat;
  cha1_hue.setter = set1_hue;
  cha1_switch_on.setter = cha1_switch_on_setter;

  cha2_on.setter = set2_on;
  cha2_bright.setter = set2_bright;
  cha2_sat.setter = set2_sat;
  cha2_hue.setter = set2_hue;
  cha2_switch_on.setter = cha2_switch_on_setter;

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
