#include "OperMgr.h"
#include "Feedback.h"

void OperMgr::setup(){
    Serial.print("Configuring Sensors and Components");
    pinMode(LED_PORT, HIGH);
    Serial.print("Sensors Configured");
}

void OperMgr::loop(Feedback** f){
    int i = 0;
    
    while(f[i] != NULL){
      int o = (*f[i]).oper;
      Serial.print("Operating operation: ");
      Serial.println(o);
      switch(o){        
        case 0:{
          digitalWrite(LED_PORT, HIGH);
          delay(1000);
          digitalWrite(LED_PORT, LOW);  
        }
        break;
      }
      i++;
    }
}
