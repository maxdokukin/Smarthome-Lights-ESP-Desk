#include <FastLED.h>

class PerlinFade{

  long lastFrame = 0;
  long counter = 0;
  
  Adafruit_NeoPixel * ledStrip;
  uint16_t ledNum;

  uint16_t hueStart, hueGap, halfHueGap;
  byte fireStep, minBright, maxBright, minSat, maxSat;

  public: 
  //(HUE_START, HUE_GAP, FIRE_STEP, MIN_BRIGHT, MAX_BRIGHT, MIN_SAT, MAX_SAT)
  PerlinFade(Adafruit_NeoPixel * ls, uint16_t ln, uint16_t hs, uint16_t hg, byte fs, byte mib, byte mab, byte mis, byte mas){

    ledStrip = ls;
    ledNum = ln;

    hueStart = hs;
    hueGap = hg;
    fireStep = fs;
    minBright = mib;
    maxBright = mab;
    minSat = mis;
    maxSat = mas;
    halfHueGap = hueGap / 2;
  }
  
  void frame(){

    if(millis() - lastFrame < 10)
      return;

    for(int i = 0; i < ledNum; i++)
      ledStrip->setPixelColor(i, getFireColor((inoise8(i * fireStep, counter))));

    counter += 5;
    
    lastFrame = millis();
  }

  long getFireColor(int val) {

    //Serial.print(HUE_START + map(val, 0, 255, 0, HUE_GAP)); Serial.print("  "); Serial.print(constrain(map(val, 0, 255, MAX_SAT, MIN_SAT), 0, 255)); Serial.print("  "); Serial.println(constrain(map(val, 0, 255, MIN_BRIGHT, MAX_BRIGHT), 0, 255));
    return ledStrip->ColorHSV(
             hueStart - halfHueGap + map(val, 0, 255, 0, hueGap),                    // H  
             constrain(map(val, 0, 255, maxSat, minSat), 0, 255),       // S
             constrain(map(val, 0, 255, minBright, maxBright), 0, 255)  // V   
           );
  }
};
