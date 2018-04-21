#include "OperMgr.h"
#include "LCDMgr.h"
#include "Feedback.h"

void OperMgr::setup(){
    mLcd.log(F("Configuring Sensors and Components"));
    pinMode(LED_PORT, HIGH);
    mLcd.log(F("Sensors Configured"));
}

void OperMgr::loop(Feedback** f){
    int i = 0;

    // Not displaying everything there is inside f
    while(f[i] != NULL){
      mLcd.log(f[i]->oper);
      delay(1000);
      i++;
    }
}
