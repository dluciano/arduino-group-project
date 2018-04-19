#include "LCDMgr.h"
#include <SoftwareSerial.h>

LCDMgr mLcd = LCDMgr();

void LCDMgr::setup() {
    Serial.println("Configuring LCD");
    lcd.begin(16, 2);
    Serial.println("LCD Configured");
}

void LCDMgr::log(String s1, String s2, bool autoscroll) {
    Serial.println(s1);
    Serial.println(s2);
    if(autoscroll)
        lcd.autoscroll();
    else
        lcd.noAutoscroll();
    lcd.clear();    
    lcd.setCursor(0, 1);    
    lcd.print(s2);

    lcd.setCursor(0, 0);    
    lcd.print(s1);
}