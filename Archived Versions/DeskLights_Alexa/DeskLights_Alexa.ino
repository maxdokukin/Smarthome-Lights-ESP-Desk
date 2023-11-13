/*
 * An example that demonstrates most capabilities of Espalexa v2.4.0
 */ 
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
//#define ESPALEXA_ASYNC            //uncomment for async operation (can fix empty body issue)
//#define ESPALEXA_NO_SUBPAGE       //disable /espalexa status page
//#define ESPALEXA_DEBUG            //activate debug serial logging
//#define ESPALEXA_MAXDEVICES 15    //set maximum devices add-able to Espalexa
#include <Espalexa.h>

// Change this!!
const char* ssid = "TheGradResident";
const char* password = "";

// prototypes
bool connectWifi();

void alexaAction(EspalexaDevice* dev);

//create devices yourself
EspalexaDevice* epsilon;

bool wifiConnected = false;

Espalexa espalexa;

void setup()
{
  Serial.begin(115200);
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(!wifiConnected){
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
  
  // Define your devices here. 
  //espalexa.addDevice("Alpha", alphaChanged, EspalexaDeviceType::onoff); //non-dimmable device
  //espalexa.addDevice("Beta", betaChanged, EspalexaDeviceType::dimmable, 127); //Dimmable device, optional 4th parameter is beginning state (here fully on)
  //espalexa.addDevice("Gamma", gammaChanged, EspalexaDeviceType::whitespectrum); //color temperature (white spectrum) device
  //espalexa.addDevice("Delta", deltaOrEpsilonChanged, EspalexaDeviceType::color); //color device
  
  epsilon = new EspalexaDevice("Epsilon", alexaAction, EspalexaDeviceType::extendedcolor); //color + color temperature
  espalexa.addDevice(epsilon);
  epsilon->setValue(128); //creating the device yourself allows you to e.g. update their state value at any time!
  
  //epsilon->setColor(200); //color temperature in mireds
  epsilon->setColor(255,160,0); //color in RGB
  //epsilon->setColor(14000,255); //color in Hue + Sat
  //epsilon->setColorXY(0.50,0.50); //color in XY

  //EspalexaDevice* d = espalexa.getDevice(3); //this will get "Delta", the index is zero-based
  //d->setPercent(50); //set value "brightness" in percent

  espalexa.begin();
}
 
void loop()
{
 espalexa.loop();
 delay(1);
}


void alexaAction(EspalexaDevice* d){

  if (d == nullptr) return;

  Serial.print("D changed to ");
  Serial.print(d->getValue());
  Serial.print(", color R");
  Serial.print(d->getR());
  Serial.print(", G");
  Serial.print(d->getG());
  Serial.print(", B");
  Serial.println(d->getB());
}


bool connectWifi(){
  bool state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
