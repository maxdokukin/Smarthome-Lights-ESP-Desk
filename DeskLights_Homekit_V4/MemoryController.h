#include <EEPROM.h>

#define MEM_CONRTROLLER_DEBUG false

class MemoryController {
  private:
    int memStart;

  public: 
    MemoryController(int ms) {
      memStart = ms;
    }

    byte* getParams() {
      byte *params = new byte[6];

      params[0] = readByteEEPROM(0); // Red
      params[1] = readByteEEPROM(1); // Green
      params[2] = readByteEEPROM(2); // Blue
      params[3] = readByteEEPROM(3); // Brightness
      params[4] = readByteEEPROM(4); // Mode
      params[5] = readByteEEPROM(5); // On/Off

      return params;
    }

    void writeByteEEPROM(int address, byte data) {
      if (MEM_CONRTROLLER_DEBUG) {
        Serial.print("Saved to EEPROM at ");
        Serial.print(memStart + address);
        Serial.print("   ");
        Serial.println(data);
      }

      EEPROM.write(memStart + address, data);
      EEPROM.commit();
    }

    byte readByteEEPROM(int address) {
      if (MEM_CONRTROLLER_DEBUG) {
        Serial.print("Got from EEPROM at ");
        Serial.print(memStart + address);
        Serial.print("  ");
        Serial.println(EEPROM.read(memStart + address));
      }

      return EEPROM.read(memStart + address);
    }
};
