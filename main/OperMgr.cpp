#include "OperMgr.h"
#include "LCDMgr.h"
#include "Feedback.h"

const char USER_LBL[]  PROGMEM = "USER: ";

const char* const LCD_LABELS[] PROGMEM = { USER_LBL };

void OperMgr::setup(){
    mLcd.log(F("Configuring Sensors and Components"));
    pinMode(LED_PORT, HIGH);
    mLcd.log(F("Sensors Configured"));
}

void OperMgr::loop(Feedback** f){
    int i = 0;
    char label[7];
    char usernameBuffer[17];
    strcpy_P(label, (char*) pgm_read_word( &LCD_LABELS[0] ));
    label[6] = '\0';
    usernameBuffer[16] = '\0';

    // Not displaying everything there is inside f
    while(f[i] != NULL) {
      sprintf(usernameBuffer, "%s%s", label, f[i]->username);
      mLcd.log(usernameBuffer, f[i]->oper);
      delay(1000);
      i++;
    }
}
