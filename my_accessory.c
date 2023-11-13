#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
    printf("accessory identify\n");
}

// Desk and ceiling perlin switches
homekit_characteristic_t desk_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t desk_switch_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 1");

homekit_characteristic_t ceiling_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t ceiling_switch_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 2");

// Desk LED characteristics
homekit_characteristic_t desk_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t desk_name = HOMEKIT_CHARACTERISTIC_(NAME, "Desk Lights");
homekit_characteristic_t desk_bright = HOMEKIT_CHARACTERISTIC_(BRIGHTNESS, 100);
homekit_characteristic_t desk_sat = HOMEKIT_CHARACTERISTIC_(SATURATION, (float) 0);
homekit_characteristic_t desk_hue = HOMEKIT_CHARACTERISTIC_(HUE, (float) 180);

// Ceiling LED characteristics
homekit_characteristic_t ceiling_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t ceiling_name = HOMEKIT_CHARACTERISTIC_(NAME, "Ceiling Lights");
homekit_characteristic_t ceiling_bright = HOMEKIT_CHARACTERISTIC_(BRIGHTNESS, 100);
homekit_characteristic_t ceiling_sat = HOMEKIT_CHARACTERISTIC_(SATURATION, (float) 0);
homekit_characteristic_t ceiling_hue = HOMEKIT_CHARACTERISTIC_(HUE, (float) 180);

// Accessories array
homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_bridge, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "XeWe Lights"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "XeWe Industries"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "17-19-66-69"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "3.0"),
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
            &desk_on,
            &desk_name,
            &desk_bright,
            &desk_sat,
            &desk_hue,
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
            &desk_switch_on,
            &desk_switch_name,
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
            &ceiling_on,
            &ceiling_name,
            &ceiling_bright,
            &ceiling_sat,
            &ceiling_hue,
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
            &ceiling_switch_on,
            &ceiling_switch_name,
            NULL
        }),
        NULL
    }),
    NULL
};

// Server configuration
homekit_server_config_t config = {
    .accessories = accessories,
    .password = "111-11-111"
};
