#include "enums.h"
#include "PerlinFade.h"
#include "MemoryController.h"

void printFreeStackSize();

class LedController{

  //strip vars
  private:
  Adafruit_NeoPixel * ledStrip;
  uint16_t ledNum;

  //frame vars
  long lastUpdate = 0;
  byte frameTime = 10;

  //color vars
  byte RGB[3]; //[0,255]
  float hue, sat, val; //hue[0,360], sat[0, 100]
  byte bright; //[0, 255]

  bool hueUpdated = false;
  bool satUpdated = false;

  PerlinFade * pf;

  //state vars
  lightsMode mode = SOLID_COLOR;
  lightsState state = OFF;
  bool newColorReceived = false;

  MemoryController *memory;

  public:
  LedController(Adafruit_NeoPixel * stp, uint16_t ln, uint16_t memoryStart){

    ledNum = ln;

    ledStrip = stp;
    ledStrip->begin();
    
                    //(HUE_START, HUE_GAP, FIRE_STEP, MIN_BRIGHT, MAX_BRIGHT, MIN_SAT, MAX_SAT)
    pf = new PerlinFade(ledStrip, ledNum, map(hue, 0, 360, 0, 65535), 10000, 10, 100, 255, 245, 255);

    memory = new MemoryController(memoryStart);

    eepromInit();
  }
  
  void frame(){

    if(state == ON){

      if(millis() - lastUpdate >= frameTime){

        switch(mode){
        
          case SOLID_COLOR:
            for(int i = 0; i < ledNum; i++)
              ledStrip->setPixelColor(i, RGB[0], RGB[1], RGB[2]);
          break;


          case PERLIN:
            if(newColorReceived){

              pf = new PerlinFade(ledStrip, ledNum, map(hue, 0, 360, 0, 65535), 15000, 10, 10, 255, 235, 255);
              newColorReceived = false;
            }

            pf->frame();

          break;
        }
      }
    }
    else if(state == OFF){

      for(int i = 0; i < ledNum; i++)
        ledStrip->setPixelColor(i, 0, 0, 0);
    }

    ledStrip->show();
  }

  public:
  void setMode(lightsMode m){

    if(LED_CONRTROLLER_DEBUG) { Serial.print("setMode : "); Serial.println(m); }

    mode = m;
    memory->writeByteEEPROM(4, static_cast<byte>(mode));
  }

  void setHue(float h){

    if(LED_CONRTROLLER_DEBUG) { Serial.print("setHue : "); Serial.println(hue); }

    hue = h;
    hueUpdated = true;
    updateColorFromHSV();
  }

  void setSat(float s){

    if(LED_CONRTROLLER_DEBUG) { Serial.print("setSat : "); Serial.println(sat); }
    
    sat = s;
    satUpdated = true;
    updateColorFromHSV();
  }

  //[0, 100.0] -> [0, 255]
  void setBrightness(float br){

    bright = map(br, 0, 100, 0, 255);

    if(LED_CONRTROLLER_DEBUG) { Serial.print("setBrightness : "); Serial.println(bright); }

    ledStrip->setBrightness(bright);
    ledStrip->show();

    memory->writeByteEEPROM(3, bright);
  }

  void setRGB(byte * rgb){
    
    if(LED_CONRTROLLER_DEBUG) { Serial.println("setRGB : "); Serial.print("R "); Serial.println(RGB[0]); Serial.print("G "); Serial.println(RGB[1]); Serial.print("B "); Serial.println(RGB[2]); }

    RGB[0] = rgb[0];
    RGB[1] = rgb[1];
    RGB[2] = rgb[2];

    newColorReceived = true;
    
    updateColorFromRGB();
  }

  void turnOn(){

    if(LED_CONRTROLLER_DEBUG) { Serial.println("State set to ON"); }

    memory->writeByteEEPROM(5, 1);
    state = ON;
  }

  void turnOff(){

    if(LED_CONRTROLLER_DEBUG) { Serial.println("State set to OFF"); }
    memory->writeByteEEPROM(5, 0);
    state = OFF;
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

  //[ON, OFF]
  bool getState(){

    if(LED_CONRTROLLER_DEBUG) { Serial.print("getState : "); Serial.println(static_cast<bool>(state)); }

    return static_cast<bool>(state);
  }

  //[SOLID_COLOR, PERLIN]
  bool getMode(){

    if(LED_CONRTROLLER_DEBUG) { Serial.print("getMode : "); Serial.println(static_cast<bool>(mode)); }

    return static_cast<bool>(mode);
  }

  
  private:

  void eepromInit(){

    //read from memory
    byte *params = memory->getParams();

    if(LED_CONRTROLLER_DEBUG) { Serial.println("eepromInit : "); Serial.print("P0 "); Serial.println(params[0]); Serial.print("P1 "); Serial.println(params[1]); Serial.print("P2 "); Serial.println(params[2]); Serial.print("P3 "); Serial.println(params[3]); Serial.print("P4 "); Serial.println(params[4]); }

    //set params 
    setRGB(params);
    setBrightness(map(params[3], 0, 255, 0, 100.0));
    
    if(params[5] == 0)
      turnOff();
    else
      turnOn();
    
    setMode(static_cast<lightsMode>(params[4]));
  }

   void updateColorFromHSV(){

    if(hueUpdated && satUpdated){

      HSV2RGB(hue, sat, 100);
      newColorReceived = true;

      memory->writeByteEEPROM(0, RGB[0]);
      memory->writeByteEEPROM(1, RGB[1]);
      memory->writeByteEEPROM(2, RGB[2]);

      hueUpdated = false;
      satUpdated = false;
    }
  }

  void updateColorFromRGB(){

    float hsv[3];
    
    RGB2HSV(RGB, hsv);

    if(LED_CONRTROLLER_DEBUG) { Serial.println("updateColorFromRGB : "); Serial.print("H : "); Serial.println(hsv[0]); Serial.print("S : "); Serial.println(hsv[1]); Serial.print("V : "); Serial.println(hsv[2]); }
    
    hsv[0] *= 100;
    hue = map(hsv[0], 0, 100, 0, 360);
    sat = 100; 
  }

  float fract(float x) { return x - int(x); }

  float mix(float a, float b, float t) { return a + (b - a) * t; }

  float step(float e, float x) { return x < e ? 0.0 : 1.0; }

  //[255, 255, 255] -> [1.0, 1.0, 1.0]
  float* RGB2HSV(byte * params, float* hsv) {

    float r = params[0] / (float) 255;
    float g = params[1] / (float) 255;
    float b = params[2] / (float) 255;

    float s = step(b, g);
    float px = mix(b, g, s);
    float py = mix(g, b, s);
    float pz = mix(-1.0, 0.0, s);
    float pw = mix(0.6666666, -0.3333333, s);
    s = step(px, r);
    float qx = mix(px, r, s);
    float qz = mix(pw, pz, s);
    float qw = mix(r, px, s);
    float d = qx - min(qw, py);
    hsv[0] = abs(qz + (qw - py) / (6.0 * d + 1e-10));
    hsv[1] = d / (qx + 1e-10);
    hsv[2] = qx;
    return hsv;
  }

  //HSV[0, 100.0] -> RGB[0, 255]
  void HSV2RGB(float h,float s,float v) {

    int i;
    float m, n, f;

    s/=100;
    v/=100;

    if(s==0){
      RGB[0]=RGB[1]=RGB[2]=round(v*255);
      return;
    }

    h/=60;
    i=floor(h);
    f=h-i;

    if(!(i&1)){
      f=1-f;
    }

    m=v*(1-s);
    n=v*(1-s*f);

    switch (i) {

      case 0: case 6:
        RGB[0]=round(v*255);
        RGB[1]=round(n*255);
        RGB[2]=round(m*255);
      break;

      case 1:
        RGB[0]=round(n*255);
        RGB[1]=round(v*255);
        RGB[2]=round(m*255);
      break;

      case 2:
        RGB[0]=round(m*255);
        RGB[1]=round(v*255);
        RGB[2]=round(n*255);
      break;

      case 3:
        RGB[0]=round(m*255);
        RGB[1]=round(n*255);
        RGB[2]=round(v*255);
      break;

      case 4:
        RGB[0]=round(n*255);
        RGB[1]=round(m*255);
        RGB[2]=round(v*255);
      break;

      case 5:
        RGB[0]=round(v*255);
        RGB[1]=round(m*255);
        RGB[2]=round(n*255);
      break;
    }
  }
};

