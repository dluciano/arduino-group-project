#include <string.h>
#include <SoftwareSerial.h>
#include "LCDMgr.h"
#include "Feedback.h"
#include "HttpResponse.h"
#include "ConnMgr.h"

using namespace openday;

#define LED_PORT 13
struct OperMgr{
  void setup(){
    mLcd.log("Configuring Sensors and Components");
    pinMode(LED_PORT, HIGH);
    mLcd.log("Sensors Configured");
  }
  void loop(Feedback** f){
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
};

ConnMgr conn;
OperMgr op;

void setup() { 
 Serial.begin(9600);
 conn.setup(); 
 op.setup();
 mLcd.log("Ready. Waiting");
}

#define  REFRESH_RATE 2000
unsigned long long last = 0;
unsigned long long prev = -REFRESH_RATE;

void loop() {
  last = millis();
  if(last - prev >= REFRESH_RATE){
    prev = last;
    conn.get("groupdev.herokuapp.com", "/rooms/1/feedbacks", 80);  
  }
  String s = conn.loop();
  if(s != ""){
    HttpResponse response(s);      
    Feedback** fbs = response.asFeedback();
    op.loop(fbs);

    int i = 0;
    while(fbs[i] != NULL){
      delete fbs[i];
      i++;
    }
    delete [] fbs;
  }
}
