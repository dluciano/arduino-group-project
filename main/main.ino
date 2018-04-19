#include <string.h>
#include <SoftwareSerial.h>
#include "LCDMgr.h"
#include "Feedback.h"
#include "HttpResponse.h"
#include "ConnMgr.h"
#include "OperMgr.h"

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
    conn.get("mi-linux.wlv.ac.uk", "/~1720362/test.php", 80);
//    conn.get("groupdev.herokuapp.com", "/rooms/1/feedbacks", 80);  
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
