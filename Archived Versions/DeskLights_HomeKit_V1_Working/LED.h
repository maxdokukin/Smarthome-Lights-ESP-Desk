#include <Adafruit_NeoPixel.h>

#define NEOPIN          D1
#define NUMPIXELS       61

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);


void updateColor();
void setTargetColor();
void setTargetBrightness(float target);
void pixelSetup();
void updateTransitions();

void updateTransitions(){

  if(brChangeTime != -1){

    if(millis() - brChangeTime <= BRIGHTNESS_SMOOTH_TIME){

      current_brightness = map(millis() - brChangeTime, 0, BRIGHTNESS_SMOOTH_TIME, oldBrightness, targetBrightness);

      //pixels.setBrightness(map(current_brightness, 0, 100, 0, 255));

      //Serial.print(brChangeTime - millis()); Serial.print(".  current_brightness  "); Serial.println(current_brightness); 
    }
    else{

      brChangeTime = -1;
      current_brightness = targetBrightness;
      //pixels.setBrightness(map(current_brightness, 0, 100, 0, 255));

      Serial.print("Brightness reached target of "); Serial.println(current_brightness);
    }
    updateColor();
  }


  if(colChangeTime != -1){

    if(millis() - colChangeTime <= COLOR_SMOOTH_TIME){

      long curCol = getMixCol(oldRGB, targetRGB, (map(millis() - colChangeTime, 0, COLOR_SMOOTH_TIME, 0, 1000) / (float) 1000));

      rgb_colors[0] = colCon(curCol, 'R');
      rgb_colors[1] = colCon(curCol, 'G');
      rgb_colors[2] = colCon(curCol, 'B');
    }
    else{

      rgb_colors[0] = colCon(targetRGB, 'R');
      rgb_colors[1] = colCon(targetRGB, 'G');
      rgb_colors[2] = colCon(targetRGB, 'B');

      colChangeTime = -1;
    }
    updateColor();
  }  
}

void pixelSetup(){

  pixels.begin(); 
  pixels.setBrightness(map(current_brightness, 0, 100, 0, 255));

  for(int i = 0; i < NUMPIXELS; i++){

    pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
  }

  pixels.show();
}






void setTargetBrightness(float target){

  oldBrightness = current_brightness;
  targetBrightness = target;
  writeIntIntoEEPROM(1506, target);
  brChangeTime = millis();

}

void setTargetColorFromHSV(float h, float s, float v){

  oldRGB = ((long(rgb_colors[0]) & 0x0000ff) << 16) | ((long(rgb_colors[1]) & 0x0000ff) << 8) | (long(rgb_colors[2]) & 0x0000ff);
  targetRGB = HSVtoRGB(h, s, v);

  Serial.print("Old color : " ); Serial.println(oldRGB, HEX);
  Serial.print("Target color : " ); Serial.println(targetRGB, HEX);

  writeIntIntoEEPROM(1500, colCon(targetRGB, 'R'));
  writeIntIntoEEPROM(1502, colCon(targetRGB, 'G'));
  writeIntIntoEEPROM(1504, colCon(targetRGB, 'B'));

  colChangeTime = millis();
}


void setTargetColorFromRGB(byte r, byte g, byte b){

  oldRGB = ((long(rgb_colors[0]) & 0x0000ff) << 16) | ((long(rgb_colors[1]) & 0x0000ff) << 8) | (long(rgb_colors[2]) & 0x0000ff);
  targetRGB = ((long(r) & 0x0000ff) << 16) | ((long(g) & 0x0000ff) << 8) | (long(b) & 0x0000ff);

  Serial.print("Old color : " ); Serial.println(oldRGB, HEX);
  Serial.print("Target color : " ); Serial.println(targetRGB, HEX);

  writeIntIntoEEPROM(1500, colCon(targetRGB, 'R'));
  writeIntIntoEEPROM(1502, colCon(targetRGB, 'G'));
  writeIntIntoEEPROM(1504, colCon(targetRGB, 'B'));

  colChangeTime = millis();
}


void updateColor(){

  //Serial.println("updateColor");

  if(received_hue || received_sat){

    //HSV2RGB(current_hue, current_sat);
    received_hue = false;
    received_sat = false;
    setTargetColorFromHSV(current_hue / 360, current_sat / 100, 255);
    syncAlexaValues(current_hue, current_sat, -1);
  }
  
  int b = map(current_brightness, 0, 100, 0, 255);
  //Serial.print("Brightness : "); Serial.println(b);
  pixels.setBrightness(b);

  //Serial.print("R : "); Serial.print(rgb_colors[0]); Serial.print("  G : "); Serial.print(rgb_colors[1]); Serial.print("  B : "); Serial.println(rgb_colors[2]); 

  for(int i = 0; i < NUMPIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
  
  pixels.show();
}