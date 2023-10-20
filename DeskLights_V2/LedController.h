#include <Adafruit_NeoPixel.h>


class LedController{

  Adafruit_NeoPixel *strip;

  byte pin;
  unsigned short ledNum;
  long RGB, HSV, oldRGB, oldHSV, targetRGB, targetHSV;
  byte brightness;
  boolean onState = false;
  boolean debug = false;

  public: LedController(Adafruit_NeoPixel *st){

    strip = st;
    if(debug)
      Serial.println("Led controller created.");
  }

  public: 
  void setup(){

    strip->begin(); 
    strip->setBrightness(getByteFromLong(HSV, 0));

    for(int i = 0; i < LED_NUM; i++)
      strip->setPixelColor(i, RGB);

    strip->show();

    if(debug)
      Serial.println("Led controller intialization finished.");
  }


  void frame(){

    for(int i = 0; i < LED_NUM; i++)
      strip->setPixelColor(i, RGB);

    strip->show();
  }


  void setData(long newRGB, long newHSV){

    RGB = newRGB;
    HSV = newHSV;
  }

  void setTargetRGB(long newRGB){

    oldRGB = RGB;
    targetRGB = newRGB;

    Serial.print("Old color : " ); Serial.println(oldRGB, HEX);
    Serial.print("Target color : " ); Serial.println(targetRGB, HEX);
  }


  void setTargetHSV(long newHSV){

    oldHSV = HSV;
    targetHSV = newHSV;

    Serial.print("Old color : " ); Serial.println(oldRGB, HSV);
    Serial.print("Target color : " ); Serial.println(targetRGB, HSV);
  }

  void setTargetBrightness(byte newBr){


  }

 

  








  void debugOn(){debug = true;}
  void debugOff(){debug = false;}






};