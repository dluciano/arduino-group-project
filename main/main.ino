#include <avr/pgmspace.h>
#include <string.h>
#include <SoftwareSerial.h>
#include "LCDMgr.h"
#include "Feedback.h"
#include "ConnMgr.h"
#include "OperMgr.h"

#define  REFRESH_RATE 2000

const char SERVER_HOST[] PROGMEM = "mi-linux.wlv.ac.uk"; // "groupdev.herokuapp.com"
const char SERVER_PATH[] PROGMEM = "/~1720362/test.php"; // "/rooms/1/feedbacks"

const char * const ENVIRONMENT [] PROGMEM = { SERVER_HOST, SERVER_PATH };

unsigned long long last = 0;
unsigned long long prev = -REFRESH_RATE;

ConnMgr conn;
OperMgr op;


void setup() { 
  Serial.begin(9600);
  conn.setup(); 
  op.setup();
  mLcd.log(F("Ready. Waiting"));
}

void loop() {
  char hostBuffer[25];
  char pathBuffer[25];

  /*
   * So far there is only one obvious problem (others might exist):
   * A few messages are appended content from previous messages, e.g.:
   * instead of 'My Message' it is being printed like 'My Messageage'.
   */

  last = millis();
  if(last - prev >= REFRESH_RATE){
    prev = last;
    strcpy_P(hostBuffer, (char*) pgm_read_word( &ENVIRONMENT[0] ));
    strcpy_P(pathBuffer, (char*) pgm_read_word( &ENVIRONMENT[1] ));

    conn.get(hostBuffer, pathBuffer, 80);
  }

  char* s = conn.loop();
  if(s != NULL) {
    Feedback** fbs = Feedback::asFeedback(s);
    free(s);
    op.loop(fbs);

    int i = 0;
    while(fbs[i] != NULL){
      delete fbs[i];
      i++;
    }
    delete [] fbs;
  }
}
