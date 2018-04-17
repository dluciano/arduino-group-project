#ifndef LCDMgr_h
#define LCDMgr_h
#endif

#include <LiquidCrystal.h>

class LCDMgr{
  private:
  LiquidCrystal lcd = LiquidCrystal(8, 9, 4, 5, 6, 7);
  
  public:
  void setup();
  void log(String s1, String s2 = "", bool autoscroll = false);
};