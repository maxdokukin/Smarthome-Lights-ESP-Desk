#include "enums.h"
#include "PerlinFade.h"

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

  public:
  LedController(Adafruit_NeoPixel * stp, uint16_t ln){

    ledNum = ln;

    ledStrip = stp; //Adafruit_NeoPixel(ledNum, pin, NEO_GRB + NEO_KHZ800);

    ledStrip->begin();
    ledStrip->setBrightness(255);
    ledStrip->clear();
    ledStrip->show();
  //(HUE_START, HUE_GAP, FIRE_STEP, MIN_BRIGHT, MAX_BRIGHT, MIN_SAT, MAX_SAT)
    pf = new PerlinFade(ledStrip, ledNum, map(hue, 0, 360, 0, 65535), 10000, 10, 100, 255, 245, 255);
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
  }


  void updateColorFromHSV(){

    if(hueUpdated && satUpdated){

      HSV2RGB(hue, sat, 100);
      newColorReceived = true;

      hueUpdated = false;
      satUpdated = false;
    }
  }


  void turnOn(){

    state = ON;
    Serial.println("State set to ON");
  }

  void turnOff(){

    state = OFF;
    Serial.println("State set to OFF");
  }


  private:
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