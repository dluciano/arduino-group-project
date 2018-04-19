#include "OperMgr.h"
#include "LCDMgr.h"
#include "Feedback.h"

void OperMgr::setup(){
    mLcd.log("Configuring Sensors and Components");
    pinMode(LED_PORT, HIGH);
    mLcd.log("Sensors Configured");
}

void OperMgr::loop(Feedback** f){
    int i = 0;
    
    while(f[i] != NULL){
      char* o = (*f[i]).oper;
      Serial.print("Operation message received: ");
      Serial.println(o);
      mLcd.log(o);
    }
}
