#include <EEPROM.h>






///////////////////
int readIntFromEEPROM(int address);
void writeIntIntoEEPROM(int address, int number);
void loadFromEEPROM();



///////////////////
void loadFromEEPROM(){

  EEPROM.begin(4096);
  rgb_colors[0] = constrain(readIntFromEEPROM(1500), 0, 255);
  rgb_colors[1] = constrain(readIntFromEEPROM(1502), 0, 255);
  rgb_colors[2] = constrain(readIntFromEEPROM(1504), 0, 255);

  float storedHSV[3];
  rgb2hsv(rgb_colors[0], rgb_colors[1], rgb_colors[2], storedHSV);

  current_brightness = constrain(readIntFromEEPROM(1506), 0, 255);
  is_on = current_brightness;
  current_sat = map(storedHSV[1], 0, 1, 0, 100);
  current_hue = map(storedHSV[0], 0, 1, 0, 100);

  /*
  if(is_on){
 
    setTargetColorFromRGB(rgb_colors[0], rgb_colors[1], rgb_colors[2]);
    setTargetBrightness(current_brightness);
  }*/

  Serial.print("Is on : "); Serial.println(is_on);
  Serial.print("current_brightness : "); Serial.println(current_brightness);
  Serial.print("current_sat : "); Serial.println(current_sat);
  Serial.print("current_hue : "); Serial.println(current_hue);


}


void writeIntIntoEEPROM(int address, int number){

  Serial.print("Saved to EEPROM at "); Serial.print(address); Serial.print("   "); Serial.println(number);

  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
  EEPROM.commit();
}


int readIntFromEEPROM(int address){

  Serial.print("Got from EEPROM at "); Serial.print(address); Serial.print("   "); Serial.println((EEPROM.read(address) << 8) + EEPROM.read(address + 1));

  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}