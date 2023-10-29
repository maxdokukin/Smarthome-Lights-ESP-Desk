#include "enums.h"
#include "PerlinFade.h"
#include "MemoryController.h"

void printFreeStackSize();

class LedController{

  //strip vars
  Adafruit_NeoPixel * ledStrip;
  uint16_t ledNum;

  //frame vars
  long lastUpdate = 0;
  byte frameTime = 10;

  //color vars
  byte RGB[3]; //[0,255]
  float hue, sat, val; //hue[0,360], sat?[0, 100]
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

    ledStrip = stp; //Adafruit_NeoPixel(ledNum, pin, NEO_GRB + NEO_KHZ800);

    ledStrip->begin();
    ledStrip->setBrightness(255);
    ledStrip->clear();
    ledStrip->show();
  //(HUE_START, HUE_GAP, FIRE_STEP, MIN_BRIGHT, MAX_BRIGHT, MIN_SAT, MAX_SAT)
    pf = new PerlinFade(ledStrip, ledNum, map(hue, 0, 360, 0, 65535), 10000, 10, 100, 255, 245, 255);

    memory = new MemoryController(memoryStart);

    eepromInit();
  }

  void eepromInit(){

    //read from memory
    byte *params = memory->getParams();

    //set params 
    setRGB(params);
    setBrightness(map(params[3], 0, 255, 0, 100.0));
    
    if(params[3] != 0)
      turnOn();
    else
      turnOff();
    
    setMode(static_cast<lightsMode>(params[4]));
  }

  
  void frame(){

    //update leds
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

    mode = m;
    memory->writeByteEEPROM(4, static_cast<byte>(mode));
  }

  void setHue(float h){

    hue = h;
    hueUpdated = true;
    updateColorFromHSV();
  }


  void setSat(float s){

    sat = s;
    satUpdated = true;
    updateColorFromHSV();
  }


  //[0, 100.0] -> [0, 255]
  void setBrightness(float br){

    bright = map(br, 0, 100, 0, 255);

    ledStrip->setBrightness(bright);
    ledStrip->show();

    memory->writeByteEEPROM(3, bright);
  }

  void setRGB(byte * rgb){

    // Serial.println("setRGB");

    // printFreeStackSize();

    // LOG_D("Free heap: %d, HomeKit clients: %d",
		// 		ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

    RGB[0] = rgb[0];
    RGB[1] = rgb[1];
    RGB[2] = rgb[2];
    newColorReceived = true;
    Serial.println("setRGB 2");
    updateColorFromRGB();
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

    float *hsv = new float[3];
    RGB2HSV(RGB, hsv);
    setHue(map(hsv[0], 0, 1, 0, 360));
    setSat(map(hsv[1], 0, 1, 0, 100));
  }


  void turnOn(){

    state = ON;
    Serial.println("State set to ON");
  }

  void turnOff(){

    state = OFF;
    Serial.println("State set to OFF");
  }

  //[0, 360]
  float getHue(){

    return hue;
  }

  //[0, 100]
  float getSat(){

    return sat;
  }

  //[0, 100]
  int getBrightness(){

    return (int) (bright / 2.55);
  }

  //[ON, OFF]
  bool getState(){

    return static_cast<bool>(state);
  }

  //[SOLID_COLOR, PERLIN]
  bool getMode(){

    return static_cast<bool>(mode);
  }

  


  private:
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


void printFreeStackSize() {

  uint32_t free_stack = ESP.getFreeContStack();
  Serial.print("Free stack: ");
  Serial.println(free_stack);
}

