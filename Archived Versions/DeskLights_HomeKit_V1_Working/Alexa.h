


void alexaSetup(){

  deskLed = new EspalexaDevice("Desk Lights", alexaAction, EspalexaDeviceType::extendedcolor); //color + color temperature
  espalexa.addDevice(deskLed);
  //deskLed->setValue(128); //creating the device yourself allows you to e.g. update their state value at any time!
  
  //deskLed->setColor(255,160,0); //color in RGB

  deskLed->setColor(rgb_colors[0], rgb_colors[1], rgb_colors[2]);
  deskLed->setValue(map(current_brightness, 0, 100, 0, 255));
  deskLed->setState(current_brightness != 0);
  
  espalexa.begin();
}



void alexaLoop(){

 espalexa.loop();



}


void alexaAction(EspalexaDevice* d){
  #include <Espalexa.h>

  if (d == nullptr) return;

  if(!is_on && d->getValue() > 0)
    syncHomekitState(true);
  else if(is_on && d->getValue() == 0)
    syncHomekitState(false);

  Serial.print("D changed to ");
  Serial.print(d->getValue());
  Serial.print(", color R");
  Serial.print(d->getR());
  Serial.print(", G");
  Serial.print(d->getG());
  Serial.print(", B");
  Serial.print(d->getB());
  Serial.print(", H");
  Serial.print(d->getHue());
  Serial.print(", S");
  Serial.println(d->getSat());

  
  syncHomekitValues(map(d->getHue(), 0, 65535, 0, 360), map(d->getSat(), 0, 255, 0, 100), map(d->getValue(), 0, 255, 0, 100));

  setTargetBrightness(d->getValue());
  setTargetColorFromRGB(d->getR(), d->getG(), d->getB());
}



