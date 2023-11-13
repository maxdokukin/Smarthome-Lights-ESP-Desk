#include <EEPROM.h>

class MemoryController{

  private:
  int memStart;

  public: MemoryController(int ms){

    memStart = ms;
  }

  byte * getParams(){

    byte *params = new byte[6];

    params[0] = readByteEEPROM(0);//r
    params[1] = readByteEEPROM(1);//g
    params[2] = readByteEEPROM(2);//b
    params[3] = readByteEEPROM(3);//brightness
    params[4] = readByteEEPROM(4);//mode
    params[5] = readByteEEPROM(5);//on/off

    return params;
  }

  void writeByteEEPROM(int address, byte data){

    Serial.print("Saved to EEPROM at "); Serial.print(memStart + address); Serial.print("   "); Serial.println(data);

    EEPROM.write(memStart + address, data);
    EEPROM.commit();
  }

  byte readByteEEPROM(int address){

    Serial.print("Got from EEPROM at "); Serial.print(memStart + address);  Serial.print("  "); Serial.println(EEPROM.read(memStart + address));
    
    return EEPROM.read(memStart + address);
  }
};