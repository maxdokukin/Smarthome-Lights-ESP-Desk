#include <FastLED.h>

#define HUE_CHANGE_TIME 900
#define PERLIN_DEBUG false

class PerlinFade {
  private:
    // Frame drawing
    uint32_t lastFrame = 0;
    uint32_t counter = 0;
  
    // LEDs
    Adafruit_NeoPixel *ledStrip;
    uint16_t ledNum;

    // Colors
    uint16_t hueStart, hueGap, adjustedHueGap, adjustedHueGapHalf, halfHueGap;
    byte fireStep, minBright, maxBright, minSat, maxSat;

    // Smooth hue change
    boolean hueChanging = false;
    uint16_t hueOld, hueTarget;
    uint32_t hueChangeStartTime;

  public:
    PerlinFade(Adafruit_NeoPixel *ls, uint16_t ln, uint16_t hs, uint16_t hg, byte fs, byte mib, byte mab, byte mis, byte mas) {
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
      
      adjustedHueGap = adjustHueGap();
      adjustedHueGapHalf = adjustedHueGap / 2;
    }
  
    void frame() {
      if (millis() - lastFrame < 10)
        return;

      hueChange();

      for (int i = 0; i < ledNum; i++)
        ledStrip->setPixelColor(i, getFireColor(inoise8(i * fireStep, counter)));

      counter += 5;
      lastFrame = millis();
    }

    void hueChange() {
      if (hueChanging) {
        uint16_t delta = millis() - hueChangeStartTime;

        if (delta <= HUE_CHANGE_TIME) {
          hueStart = hueOld + (hueTarget - hueOld) * (delta / (float)HUE_CHANGE_TIME);
        } else {
          if (PERLIN_DEBUG) {
            Serial.print("hueChange : done. ");
            Serial.println(hueTarget);
            Serial.print("time : ");
            Serial.println(millis());
          }

          hueStart = hueTarget;
          hueChanging = false;
          hueChangeStartTime = 0;
          adjustedHueGap = adjustHueGap();
          adjustedHueGapHalf = adjustedHueGap / 2;
        }
      }
    }

    void setNewHue(uint16_t ht) {
      hueTarget = ht;
      hueOld = hueStart;
      hueChangeStartTime = millis();
      hueChanging = true;

      if (PERLIN_DEBUG) {
        Serial.print("setNewHue : ");
        Serial.println(ht);
        Serial.print("hueOld : ");
        Serial.println(hueOld);
        Serial.print("hueChangeStartTime : ");
        Serial.println(hueChangeStartTime);
        Serial.print("millis : ");
        Serial.println(millis());
        Serial.print("hueChanging : ");
        Serial.println(hueChanging);
      }
    }

    long getFireColor(int val) {
      return ledStrip->ColorHSV(
        hueStart - adjustedHueGapHalf + map(val, 0, 255, 0, adjustedHueGap),
        constrain(map(val, 0, 255, maxSat, minSat), 0, 255),
        constrain(map(val, 0, 255, minBright, maxBright), 0, 255)
      );
    }

    uint16_t adjustHueGap() {
      if (hueStart > 55000 || hueStart < 10000)
        return hueGap * 0.2;
      else 
        return hueGap;
    }
};
