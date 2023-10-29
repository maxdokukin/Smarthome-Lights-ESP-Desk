#include <EEPROM.h>


class MemoryController{

  private:
  int memStart;

  public: MemoryController(int ms){

    EEPROM.begin(4096);
    memStart = ms;
  }

  byte * getParams(){

    byte *params = new byte[5];

    params[0] = readByteEEPROM(0);//r
    params[1] = readByteEEPROM(1);//g
    params[2] = readByteEEPROM(2);//b
    params[3] = readByteEEPROM(3);//brightness
    params[4] = readByteEEPROM(4);//mode

    return params;
  }




  // void writeIntEEPROM(int address, int number){

  //   Serial.print("Saved to EEPROM at "); Serial.print(address); Serial.print("   "); Serial.println(number);

  //   EEPROM.write(address, number >> 8);
  //   EEPROM.write(address + 1, number & 0xFF);
  //   EEPROM.commit();
  // }

  // int readIntEEPROM(int address){

  //   Serial.print("Got from EEPROM at "); Serial.print(address); Serial.print("   "); Serial.println((EEPROM.read(address) << 8) + EEPROM.read(address + 1));

  //   return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
  // }

  void writeByteEEPROM(int address, byte data){

    Serial.print("Saved to EEPROM at "); Serial.print(memStart + address); Serial.print("   "); Serial.println(data);

    EEPROM.write(address, data);
    EEPROM.commit();
  }

  byte readByteEEPROM(int address){

    Serial.print("Got from EEPROM at "); Serial.print(memStart + address);  Serial.print("  "); Serial.println(EEPROM.read(address));
    
    return EEPROM.read(address);
  }








};