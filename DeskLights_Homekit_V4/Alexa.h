#include <Espalexa.h>

//callback functions
//new callback type, contains device pointer
void dl(EspalexaDevice* dev);
void dlf(EspalexaDevice* dev);
void cl(EspalexaDevice* dev);
void clf(EspalexaDevice* dev);

Espalexa espalexa;

void setupAlexa(){

  // Define your devices here. 
  espalexa.addDevice("Desk Lights", dl, EspalexaDeviceType::color); //color device
  espalexa.addDevice("Desk Lights Fade", dlf, EspalexaDeviceType::onoff); //non-dimmable device
  espalexa.addDevice("Ceiling Lights", cl, EspalexaDeviceType::color); //color device
  espalexa.addDevice("Ceiling Lights Fade", clf, EspalexaDeviceType::onoff); //non-dimmable device
  
  espalexa.begin();
}

void dl(EspalexaDevice* d){

  if (d == nullptr) return;

  deskLights->setRGB(d->getRGB());
  deskLights->setBrightness(d->getValue() / (float) 2.55);

  if(d->getState() != deskLights->getState())
    deskLights->setState(d->getState());  
}

void dlf(EspalexaDevice* d) {
  
  if (d == nullptr) return; //this is good practice, but not required

  deskLights->setMode(d->getValue() ? PERLIN : SOLID_COLOR);
}

void cl(EspalexaDevice* d){

  if (d == nullptr) return;

  ceilingLights->setRGB(d->getRGB());
  ceilingLights->setBrightness(d->getValue() / (float) 2.55);

  if(d->getState() != ceilingLights->getState())
    ceilingLights->setState(d->getState());
}

void clf(EspalexaDevice* d) {

  if (d == nullptr) return; //this is good practice, but not required

  ceilingLights->setMode(d->getValue() ? PERLIN : SOLID_COLOR);
}

void syncValuesAlexa(){

  espalexa.getDevice(0)->setState(deskLights->getState());
  espalexa.getDevice(0)->setValue(map(deskLights->getBrightness(),0, 100, 0, 255));
  espalexa.getDevice(0)->setColor(deskLights->getR(), deskLights->getG(), deskLights->getB());

  espalexa.getDevice(1)->setState(deskLights->getMode());

  espalexa.getDevice(2)->setState(ceilingLights->getState());
  espalexa.getDevice(2)->setValue(map(ceilingLights->getBrightness(),0, 100, 0, 255));
  espalexa.getDevice(2)->setColor(ceilingLights->getR(), ceilingLights->getG(), ceilingLights->getB());

  espalexa.getDevice(3)->setState(ceilingLights->getMode());
}