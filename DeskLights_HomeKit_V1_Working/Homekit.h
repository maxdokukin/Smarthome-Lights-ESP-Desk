static uint32_t next_heap_millis = 0;


void homekitLoop() {

	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		//LOG_D("Free heap: %d, HomeKit clients: %d",
    //ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}

void set_on(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_on.value.bool_value = on; //sync the value

    if(on) {
        is_on = true;
        Serial.println("On");
        syncAlexaState(true);

        if(!targetBrightness)
          setTargetBrightness(oldBrightness);
    } else  {
        is_on = false;
        Serial.println("Off");
        syncAlexaState(false);
        setTargetBrightness(0);
    }

    updateColor();
}
//0-360
void set_hue(const homekit_value_t v) {

    Serial.println("set_hue to "); Serial.println(v.float_value);
    float hue = v.float_value;
    cha_hue.value.float_value = hue; //sync the value

    current_hue = hue;
    received_hue = true;
    
    updateColor();
}
//0-100
void set_sat(const homekit_value_t v) {

    Serial.print("set_sat to "); Serial.println(v.float_value);
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value

    current_sat = sat;
    received_sat = true;
    
    updateColor();
}
//0-100
void set_bright(const homekit_value_t v) {

  Serial.println("set_bright to "); Serial.println(v.int_value);
  int bright = v.int_value;
  cha_bright.value.int_value = bright; //sync the value

  setTargetBrightness(bright);  
  
  updateColor();
  syncAlexaValues(-1, -1, bright);
}

void my_homekit_setup() {

  cha_on.setter = set_on;
  cha_bright.setter = set_bright;
  cha_sat.setter = set_sat;
  cha_hue.setter = set_hue;

  cha_on.value.bool_value = current_brightness;
  cha_hue.value.float_value = current_hue; 
  cha_sat.value.float_value = current_sat; 
  cha_bright.value.int_value = current_brightness;
  
	arduino_homekit_setup(&accessory_config);
}


