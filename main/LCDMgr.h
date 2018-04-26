#ifndef LCDMgr_h
#define LCDMgr_h
#endif

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <LiquidCrystal.h>

class LCDMgr{  
  public:
  void setup();
  void log(String s1, String s2 = "", bool autoscroll = false);
  void log(char *s1, char *s2, bool autoscroll = false);
};

extern LCDMgr mLcd;
