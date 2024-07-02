#include "enums.h"
#include "Color.h"
#include "PerlinFade.h"
#include "MemoryController.h"

#define SOLID_COLOR_TRANSITION_TIME 900
#define BRIGHTNESS_TRANSITION_TIME 900

#define LED_CONRTROLLER_DEBUG true

class LedController{

  private:
  //strip vars
  Adafruit_NeoPixel * ledStrip;
  uint16_t ledNum;

  //frame vars
  uint32_t lastUpdate = 0;
  byte frameTime = 10;

  //color vars
  byte RGB[3]; //[0,255]
  float hue, sat, val; //hue[0,360], sat[0, 100], val[0, 100]
  byte bright; //[0, 255]

  bool hueUpdated = false;
  bool satUpdated = false;

  PerlinFade * pf;

  //state vars
  lightsMode mode = SOLID_COLOR;
  lightsState state = OFF;
  bool newColorReceived = false;

  //smooth brightness
  boolean brighnessChanging = false;
  uint32_t brightnessStartTime = 0;
  byte oldBrightness = 0;
  byte targetBrightness = 0;

  //smooth color transition
  uint32_t transitionStartTime = 0;
  byte oldRGB[3]; //[0, 255]

  MemoryController *memory;

  public:
  LedController(Adafruit_NeoPixel * stp, uint16_t ln, uint16_t memoryStart){
    ledNum = ln;

    ledStrip = stp;
    ledStrip->begin();

    memory = new MemoryController(memoryStart);
                       //(HUE_START, HUE_GAP, FIRE_STEP, MIN_BRIGHT, MAX_BRIGHT, MIN_SAT, MAX_SAT)
    pf = new PerlinFade(ledStrip, ledNum, map(hue, 0, 360, 0, 65535), 10000, 10, 100, 255, 245, 255);//potentioally set a new color here.

    eepromInit();
  }
  
  void frame(){
    
    if(state == ON){
      
      if(millis() - lastUpdate >= frameTime){
        
        switch(mode){

          case SOLID_COLOR:{

            if(newColorReceived){

              transitionStartTime = millis();
              newColorReceived = false;
              mode = SOLID_COLOR_TRANSITION;
              return;
            }

            for(int i = 0; i < ledNum; i++)
              ledStrip->setPixelColor(i, RGB[0], RGB[1], RGB[2]);
          break;
          }

          case SOLID_COLOR_TRANSITION: {

            int delta = millis() - transitionStartTime;

            if(delta >= SOLID_COLOR_TRANSITION_TIME){

              mode = SOLID_COLOR;
              return;
            }

            long transitionColor = getMixCol((oldRGB[0] << 16) | (oldRGB[1] << 8) | oldRGB[2], (RGB[0] << 16) | (RGB[1] << 8) | RGB[2], delta / (float) SOLID_COLOR_TRANSITION_TIME);

            for(int i = 0; i < ledNum; i++)
              ledStrip->setPixelColor(i, colCon(transitionColor, 'R'), colCon(transitionColor, 'G'), colCon(transitionColor, 'B'));

          break;
          }

          case PERLIN:{
            if(newColorReceived)
              newColorReceived = false;

            pf->frame();

          break;
          }
        }
      }
    }
    else if(state == OFF){

    }

    controlBrightness();

    ledStrip->show();
  }

  public:
  void setMode(lightsMode m){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("setMode : "); Serial.println(m); }

    mode = m;
    memory->writeByteEEPROM(4, static_cast<byte>(mode));

    valuesUpdated = true;
  }

  //[0, 360.0]
  void setHue(float h){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("setHue : "); Serial.println(hue); }

    hue = h;
    hueUpdated = true;
    updateColorFromHSV();
  }

  //[0, 100.0]
  void setSat(float s){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("setSat : "); Serial.println(sat); }
    
    sat = s;
    satUpdated = true;

    //NEW CODE
    if(s < 35.0 && mode == PERLIN){
      setMode(SOLID_COLOR)
    }
    //NEW CODE

    updateColorFromHSV();
  }

  //[0, 100.0] -> [0, 255]
  void setBrightness(float br){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("setBrightness : "); Serial.println(br); }

    setTragetBrightness(map(br, 0, 100, 0, 255));

    memory->writeByteEEPROM(3, targetBrightness);

    valuesUpdated = true;
  }

  //[0, 255]
  void setRGB(byte * rgb){
    oldRGB[0] = RGB[0];
    oldRGB[1] = RGB[1];
    oldRGB[2] = RGB[2];

    RGB[0] = rgb[0];
    RGB[1] = rgb[1];
    RGB[2] = rgb[2];

    if(LED_CONRTROLLER_DEBUG) { Serial.println("setRGB : "); Serial.print("R "); Serial.println(RGB[0]); Serial.print("G "); Serial.println(RGB[1]); Serial.print("B "); Serial.println(RGB[2]); }
    
    //NEW CODE
    if((RGB[0] == RGB[1] == RGB[2]) && mode == PERLIN){
      setMode(SOLID_COLOR)
    }
    //NEW CODE

    updateColorFromRGB();
  }

  //[0, 255]
  void setRGB(long rgb){
    oldRGB[0] = RGB[0];
    oldRGB[1] = RGB[1];
    oldRGB[2] = RGB[2];
    
    RGB[0] = ((rgb >> 16) & 0xFF);
    RGB[1] = ((rgb >> 8) & 0xFF);
    RGB[2] = (rgb & 0xFF);

    if(LED_CONRTROLLER_DEBUG) { Serial.println("setRGB : "); Serial.print("R "); Serial.println(RGB[0]); Serial.print("G "); Serial.println(RGB[1]); Serial.print("B "); Serial.println(RGB[2]); }

    //NEW CODE
    if((RGB[0] == RGB[1] == RGB[2]) && mode == PERLIN){
      setMode(SOLID_COLOR)
    }
    //NEW CODE

    updateColorFromRGB();
  }
  //[ON, OFF]
  void setState(boolean s){
    if(s)
      turnOn();
    else
      turnOff();
  }

  void turnOn(){
    if(state == ON)
      return;

    if(LED_CONRTROLLER_DEBUG) { Serial.println("State set to ON"); }

    memory->writeByteEEPROM(5, 1);
    state = ON;

    if(!brighnessChanging)
      setTragetBrightness(oldBrightness);

    valuesUpdated = true;
  }

  void turnOff(){
    if(state == OFF)
      return;
      
    if(LED_CONRTROLLER_DEBUG) { Serial.println("State set to OFF"); }
    memory->writeByteEEPROM(5, 0);
    state = OFF;
    setTragetBrightness(0);

    valuesUpdated = true;
  }

  //[0, 360]
  float getHue(){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("getHue : "); Serial.println(hue); }

    return hue;
  }

  //[0, 100]
  float getSat(){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("getSat : "); Serial.println(sat); }

    return sat;
  }

  //[0, 100]
  int getBrightness(){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("getBrightness : "); Serial.println((int) (bright / 2.55)); }

    return (int) (bright / 2.55);
  }

  //[0, 100]
  int getTargetBrightness(){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("getTargetBrightness : "); Serial.println((int) (targetBrightness / 2.55)); }

    return (int) (targetBrightness / 2.55);
  }

  //[ON, OFF]
  bool getState(){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("getState : "); Serial.println(static_cast<bool>(state)); }

    return static_cast<bool>(state);
  }

  //[SOLID_COLOR, PERLIN, SOLID_COLOR_TRANSITION]
  bool getMode(){
    if(LED_CONRTROLLER_DEBUG) { Serial.print("getMode : "); Serial.println(static_cast<bool>(mode)); }

    return static_cast<bool>(mode);
  }

  //[0, 255]
  byte getR() { 
    if(LED_CONRTROLLER_DEBUG) { Serial.print("getR : "); Serial.println(RGB[0]); }

    return RGB[0]; 
  }

  //[0, 255]
  byte getG() { 
    if(LED_CONRTROLLER_DEBUG) { Serial.print("getG : "); Serial.println(RGB[1]); }

    return RGB[1]; 
  }

  //[0, 255]
  byte getB() { 
    if(LED_CONRTROLLER_DEBUG) { Serial.print("getB : "); Serial.println(RGB[2]); }

    return RGB[2]; 
  }

  private:
  void setTragetBrightness(byte target){
    oldBrightness = bright;
    targetBrightness = target;
    brighnessChanging = true;    
  }

  void controlBrightness(){
    if(!brighnessChanging)
      return;

    if(brightnessStartTime == -1)
      brightnessStartTime = millis();

    unsigned int delta = millis() - brightnessStartTime;

    if(delta <= BRIGHTNESS_TRANSITION_TIME){

      bright = oldBrightness + (targetBrightness - oldBrightness) * (delta / (float) BRIGHTNESS_TRANSITION_TIME);
    }
    else{

      bright = targetBrightness;
      brighnessChanging = false;
      brightnessStartTime = -1;
    }

    ledStrip->setBrightness(bright);
  }

  void eepromInit(){
    byte *params = memory->getParams();

    if(LED_CONRTROLLER_DEBUG) { Serial.println("eepromInit : "); Serial.print("P0 "); Serial.println(params[0]); Serial.print("P1 "); Serial.println(params[1]); Serial.print("P2 "); Serial.println(params[2]); Serial.print("P3 "); Serial.println(params[3]); Serial.print("P4 "); Serial.println(params[4]); }

    setRGB(params);
    oldBrightness = targetBrightness = params[3];
    
    setState(params[5]);
    
    setMode(static_cast<lightsMode>(params[4]));
  }

   void updateColorFromHSV(){
    if(hueUpdated && satUpdated){

      oldRGB[0] = RGB[0];
      oldRGB[1] = RGB[1];
      oldRGB[2] = RGB[2];

      HSV2RGB(hue, sat, 100, RGB);

      pf->setNewHue(map(hue, 0, 360, 0, 65535));

      memory->writeByteEEPROM(0, RGB[0]);
      memory->writeByteEEPROM(1, RGB[1]);
      memory->writeByteEEPROM(2, RGB[2]);

      hueUpdated = false;
      satUpdated = false;
      newColorReceived = true;
      valuesUpdated = true;
    }
  }

  void updateColorFromRGB(){
    float hsv[3];
    
    RGB2HSV(RGB, hsv);

    if(LED_CONRTROLLER_DEBUG) { Serial.println("updateColorFromRGB : "); Serial.print("H : "); Serial.println(hsv[0]); Serial.print("S : "); Serial.println(hsv[1]); Serial.print("V : "); Serial.println(hsv[2]); }
    
    hsv[0] *= 100;
    hue = map(hsv[0], 0, 100, 0, 360);
    sat = 100; 
                           //(HUE_START, HUE_GAP, FIRE_STEP, MIN_BRIGHT, MAX_BRIGHT, MIN_SAT, MAX_SAT)
    pf->setNewHue(map(hue, 0, 360, 0, 65535));
  
    memory->writeByteEEPROM(0, RGB[0]);
    memory->writeByteEEPROM(1, RGB[1]);
    memory->writeByteEEPROM(2, RGB[2]);     

    newColorReceived = true;
    valuesUpdated = true;
  }
};