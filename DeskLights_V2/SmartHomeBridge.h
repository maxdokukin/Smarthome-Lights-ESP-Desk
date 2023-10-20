class SmartHomeBridge{

  LedController * led;
  MemoryController * memory;
  AlexaController * alexa;
  HomekitController * homekit;

  public: SmartHomeBridge(LedController * lc, MemoryController * mc, AlexaController *ac, HomekitController *hc){

    led = lc;
    memory = mc;
    alexa = ac;
    homekit = hc;
  }


  public:
  void setup(){


  }


  void frame(){

    // alexa->frame();
    // homekit->frame();
    led->frame();
  }


  void alexaReceivedData(long RGB, long HSV){

    led->setTargetRGB(RGB);
    led->setTargetHSV(HSV);
    homekit->updateData(HSV);
  }
};
