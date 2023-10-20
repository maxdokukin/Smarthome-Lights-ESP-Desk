/*
 * my_accessory.c
 * Define the accessory in C language using the Macro in characteristics.h
 *
 *  Created on: 2020-05-16
 *      Author: Mixiaoxiao (Wang Bin)
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

// format: bool; HAP section 9.70; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t cha1_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha1_switch_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 1");
homekit_characteristic_t cha2_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha2_switch_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 2");

//led
homekit_characteristic_t cha1_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha1_name = HOMEKIT_CHARACTERISTIC_(NAME, "Desk Lights");
homekit_characteristic_t cha1_bright = HOMEKIT_CHARACTERISTIC_(BRIGHTNESS, 100);
homekit_characteristic_t cha1_sat = HOMEKIT_CHARACTERISTIC_(SATURATION, (float) 0);
homekit_characteristic_t cha1_hue = HOMEKIT_CHARACTERISTIC_(HUE, (float) 180);

homekit_characteristic_t cha2_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha2_name = HOMEKIT_CHARACTERISTIC_(NAME, "Ceiling Lights");
homekit_characteristic_t cha2_bright = HOMEKIT_CHARACTERISTIC_(BRIGHTNESS, 100);
homekit_characteristic_t cha2_sat = HOMEKIT_CHARACTERISTIC_(SATURATION, (float) 0);
homekit_characteristic_t cha2_hue = HOMEKIT_CHARACTERISTIC_(HUE, (float) 180);

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_bridge, .services=(homekit_service_t*[]) {
    	// HAP section 8.17:
    	// For a bridge accessory, only the primary HAP accessory object must contain this(INFORMATION) service.
    	// But in my test,
    	// the bridged accessories must contain an INFORMATION service,
    	// otherwise the HomeKit will reject to pair.
    	HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "XeWe Lights"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "XeWe Industries"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "6660666"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        NULL
    }),
    HOMEKIT_ACCESSORY(.id=2, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[]) {
          HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
              HOMEKIT_CHARACTERISTIC(NAME, "Desk Lights"),
              HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
              NULL
          }),
          HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
              &cha1_on,
              &cha1_name,
              &cha1_bright,
              &cha1_sat,
              &cha1_hue,
              NULL
          }),
          NULL
      }),

    HOMEKIT_ACCESSORY(.id=3, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Desk Lights Fade"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
		HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&cha1_switch_on,
			&cha1_switch_name,
			NULL
		}),
        NULL
    }),
    HOMEKIT_ACCESSORY(.id=4, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[]) {
          HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
              HOMEKIT_CHARACTERISTIC(NAME, "Ceiling Lights"),
              HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
              NULL
          }),
          HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
              &cha2_on,
              &cha2_name,
              &cha2_bright,
              &cha2_sat,
              &cha2_hue,
              NULL
          }),
          NULL
      }),

    HOMEKIT_ACCESSORY(.id=5, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Ceiling Lights Fade"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
		HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&cha2_switch_on,
			&cha2_switch_name,
			NULL
		}),
        NULL
    }),
    NULL
};


homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};

