#include "lightsMode.h"


class LedController{

  //strip info
  Adafruit_NeoPixel ledStrip;
  byte pin;
  uint16_t ledNum;

  //frame drawing
  lightsMode mode = SOLID_COLOR;
  long lastUpdate = 0;
  byte frameTime = 10;

  //colors
  byte RGB[3]; //[0,255]
  float hue, sat, val; //[0,100]
  float bright; //[0, 100]
  
  LedController(uint16_t ln, byte p){

    ledNum = ln;
    pin = p;

    ledStrip = Adafruit_NeoPixel(ledNum, pin, NEO_GRB + NEO_KHZ800);

    ledStrip.begin();
    ledStrip.setBrightness(255);
    ledStrip.clear();
    ledStrip.show();
  }

  
  void frame(){

    //update leds

    if(millis() - lastUpdate >= frameTime){

      switch(mode){
      
        case SOLID_COLOR:
          for(int i = 0; i < ledNum; i++)
            ledStrip.setPixelColor(i, RGB[0], RGB[1], RGB[2]);
        break;


        case PERLIN:


        break;
      }

      ledStrip.show();
    }
  }


  void updateColorsFromHSV(float h, float s, float v){

    HSV2RGB(h, s, v);
    newColorReceived = true;

  }



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








};