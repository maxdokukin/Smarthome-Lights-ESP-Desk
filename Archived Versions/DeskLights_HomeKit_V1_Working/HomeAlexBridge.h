extern "C" homekit_server_config_t accessory_config;
extern "C" homekit_characteristic_t cha_on;
extern "C" homekit_characteristic_t cha_bright;
extern "C" homekit_characteristic_t cha_sat;
extern "C" homekit_characteristic_t cha_hue;

#include <Espalexa.h>

void alexaAction(EspalexaDevice* dev);

EspalexaDevice* deskLed;
Espalexa espalexa;


void syncHomekitValues(float h, float s, float v){

  Serial.println("Syncing Homekit from Alexa");
  Serial.print("H : "); Serial.println(h);
  Serial.print("S : "); Serial.println(s);
  Serial.print("V : "); Serial.println(v);

  cha_hue.value.float_value = h; 
  cha_sat.value.float_value = s; 
  cha_bright.value.int_value = v;
}

void syncHomekitState(boolean state){

  is_on = state;
  cha_on.value.bool_value = state;
}

void syncAlexaValues(int h, int s, int v){

  if(v >= 0)
    deskLed->setValue(map(v, 0, 100, 0, 255)); //creating the device yourself allows you to e.g. update their state value at any time!
  
  if(h >= 0 && s >= 0)  
    deskLed->setColor(map(h, 0, 360, 0, 65535), map(s, 0, 100, 0, 254));

  Serial.println("Syncing Alexa from Homekit");
  Serial.print("H : "); Serial.print(h); Serial.print(" : "); Serial.println(map(h, 0, 360, 0, 65535));
  Serial.print("S : "); Serial.print(s); Serial.print(" : "); Serial.println(map(s, 0, 100, 0, 255));
  Serial.print("V : "); Serial.print(v); Serial.print(" : "); Serial.println(map(v, 0, 100, 0, 255));
}

void syncAlexaState(boolean state){

  deskLed->setState(state);
}

