#include "LCDMgr.h"
#include <SoftwareSerial.h>

LiquidCrystal lcd = LiquidCrystal(8, 9, 4, 5, 6, 7);
LCDMgr mLcd;

void LCDMgr::setup() {
    Serial.println(F("Configuring LCD"));
    lcd.begin(16, 2);
    Serial.println(F("LCD Configured"));
}

void LCDMgr::log(String s1, String s2, bool autoscroll) {
    if(autoscroll)
        lcd.autoscroll();
    else
        lcd.noAutoscroll();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(s1);

    lcd.setCursor(0, 1);
    lcd.print(s2);
}

void LCDMgr::log(char *s1, char *s2, bool autoscroll) {
    if(autoscroll)
        lcd.autoscroll();
    else
        lcd.noAutoscroll();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(s1);

    lcd.setCursor(0, 1);
    lcd.print(s2);

    Serial.print(F("LOGGED L1::: "));
    Serial.println(s1);
    Serial.print(F("LOGGED L2::: "));
    Serial.println(s2);
}

