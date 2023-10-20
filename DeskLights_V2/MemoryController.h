#include <EEPROM.h>

class MemoryController{

  unsigned short variableStorageAddress;
  boolean debug = false;

  public: MemoryController(unsigned short address){

    variableStorageAddress = address;

    if(debug)
      Serial.println("Memory controller created.");
  }

  public:
  void loadFromEEPROM(LedController *ledController){

    EEPROM.begin(4096);
    byte R = readByte(variableStorageAddress);
    byte G = readByte(variableStorageAddress + 1);
    byte B = readByte(variableStorageAddress + 2);
    byte brightness = readByte(variableStorageAddress + 3);
    boolean on = brightness;


    byte HSV[3];
    rgb2hsv(R, G, B, HSV);

    ledController->setData(getLong(R, G, B), getLong(HSV[0], HSV[1], 255));

    if(debug){

      Serial.print("Is on : "); Serial.println(on);
      Serial.print("Brightness : "); Serial.println(brightness);
      Serial.print("R : "); Serial.println(R);
      Serial.print("G : "); Serial.println(G);
      Serial.print("B : "); Serial.println(B);
      Serial.print("HUE : "); Serial.println(HSV[0]);
      Serial.print("SAT : "); Serial.println(HSV[1]);
    }
  }


  void saveByte(uint16_t address, byte number){

    if(debug)
     {Serial.print("Saved to EEPROM at "); Serial.print(address); Serial.print("   "); Serial.println(number);}

    EEPROM.write(address, number);
    EEPROM.commit();
  }


  byte readByte(uint16_t address){
    
    if(debug)
     {Serial.print("Got from EEPROM at "); Serial.print(address); Serial.print("   "); Serial.println(EEPROM.read(address));}

    return EEPROM.read(address);
  }

  void debugOn(){debug = true;}
  void debugOff(){debug = false;}

};