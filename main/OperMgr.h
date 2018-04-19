#ifndef OperMgr_h
#define OperMgr_h
#endif

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

typedef class Feedback;

#define LED_PORT 13
struct OperMgr{
  void setup();
  void loop(Feedback** f);
};
