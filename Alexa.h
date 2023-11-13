#include <Espalexa.h>

void dl(EspalexaDevice* dev);
void dlf(EspalexaDevice* dev);
void cl(EspalexaDevice* dev);
void clf(EspalexaDevice* dev);

Espalexa espalexa;

void setupAlexa() {
  espalexa.addDevice("Desk Lights", dl, EspalexaDeviceType::color); // Color device
  espalexa.addDevice("Desk Lights Fade", dlf, EspalexaDeviceType::onoff); // Non-dimmable device
  espalexa.addDevice("Ceiling Lights", cl, EspalexaDeviceType::color); // Color device
  espalexa.addDevice("Ceiling Lights Fade", clf, EspalexaDeviceType::onoff); // Non-dimmable device
  
  espalexa.begin();
}

void dl(EspalexaDevice* d) {
  if (d == nullptr) return;

  deskLights->setRGB(d->getRGB());
  deskLights->setBrightness(d->getValue() / (float) 2.55);

  if (d->getState() != deskLights->getState())
    deskLights->setState(d->getState());  
}

void dlf(EspalexaDevice* d) {
  if (d == nullptr) return;

  deskLights->setMode(d->getValue() ? PERLIN : SOLID_COLOR);
}

void cl(EspalexaDevice* d) {
  if (d == nullptr) return;

  ceilingLights->setRGB(d->getRGB());
  ceilingLights->setBrightness(d->getValue() / (float) 2.55);

  if (d->getState() != ceilingLights->getState())
    ceilingLights->setState(d->getState());
}

void clf(EspalexaDevice* d) {
  if (d == nullptr) return;

  ceilingLights->setMode(d->getValue() ? PERLIN : SOLID_COLOR);
}

void syncValuesAlexa() {
  espalexa.getDevice(0)->setState(deskLights->getState());
  espalexa.getDevice(0)->setValue(map(deskLights->getTargetBrightness(), 0, 100, 0, 255));
  espalexa.getDevice(0)->setColor(deskLights->getR(), deskLights->getG(), deskLights->getB());

  espalexa.getDevice(1)->setState(deskLights->getMode());

  espalexa.getDevice(2)->setState(ceilingLights->getState());
  espalexa.getDevice(2)->setValue(map(ceilingLights->getTargetBrightness(), 0, 100, 0, 255));
  espalexa.getDevice(2)->setColor(ceilingLights->getR(), ceilingLights->getG(), ceilingLights->getB());

  espalexa.getDevice(3)->setState(ceilingLights->getMode());
}
