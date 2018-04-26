#include "ConnMgr.h"
#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include "LCDMgr.h"
SoftwareSerial espSerial(3, 2);
WiFiEspClient client;

// "Maria y Dawlin iPhone", "DA@190_MCMS3_90" ssid_length: 22 - pass lenght: 15
// StudentCom - ssid_length: 11 passBuffer: 0
const int WIFI_SSID_LENGHT PROGMEM = 11;
const int WIFI_PASS_LENGHT PROGMEM = 1;

const char WIFI_SSID[] PROGMEM = "StudentCom";
const char WIFI_PASS[] PROGMEM = "";

const char HTTP_METHOD[]    PROGMEM = "GET ";
const char HTTP_VERSION[]   PROGMEM = " HTTP/1.1";
const char HTTP_HOST_LBL[]  PROGMEM = "Host: ";

const char* const WIFI[] PROGMEM = { WIFI_SSID, WIFI_PASS };
const char* const HTTP[] PROGMEM = { HTTP_METHOD, HTTP_VERSION, HTTP_HOST_LBL };


void ConnMgr::setup(){
    // Start esp wifi and its serial interface
    mLcd.log(F("Configuring WIFI"));
    espSerial.begin(9600);
    WiFi.init(&espSerial);
    connect();
    mLcd.log(F("WIFI Configured"));
}

void ConnMgr::connect() {
    char ssidBuffer[WIFI_SSID_LENGHT];
    char passBuffer[WIFI_PASS_LENGHT];

    strcpy_P(ssidBuffer, (char*) pgm_read_word( &WIFI[0] ));
    strcpy_P(passBuffer, (char*) pgm_read_word( &WIFI[1] ));

    mLcd.log(F("Connecting to"), F("WiFi..."), false);
    if (WiFi.status() != WL_NO_SHIELD) {
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssidBuffer, passBuffer);
      }
    }
    if(WiFi.status() == WL_CONNECTED)
      mLcd.log(F("Ready. Waiting"), F(""), false);
    connected = WiFi.status() == WL_CONNECTED;
}

void ConnMgr::get(char *server, char *path, int port){
    char method[5];
    char version[10];
    char host[7];
    char line[32];

    if(WiFi.status() != WL_CONNECTED) {
      connected = false;
      mLcd.log(F("Reconnect Wifi!!"), F(""), false);
      connect();
      return;
    }

    if (!requestDone && client.connect(server, port)) {
        Serial.println(F("Connected to server"));

        strcpy_P(method, (char*) pgm_read_word( &HTTP[0] ));
        strcpy_P(version, (char*) pgm_read_word( &HTTP[1] ));
        strcpy_P(host, (char*) pgm_read_word( &HTTP[2] ));

        // Manually write the line,
        strcpy(line, method);
        strcat(line, path);
        strcat(line, version);
        line[31] = '\0';
        // then Print to the request body
        client.println(line);

        // To reuse the line variable, first we empty it
        line[0] = '\0';

        // Then, manually write the second line,
        strcpy(line, host);
        strcat(line, server);
        line[31] = '\0';
        // and finally print to the request body
        client.println(line);

        // Serial.println("\nGET REQUEST: ");
        // Serial.println(getStr);

        // Serial.println("\nHOST REQUEST: ");
        // Serial.println(host);

        client.println(F("Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJsdW1lbi1qd3QiLCJzdWIiOm51bGwsImlhdCI6MTUyNDA5MzA3OSwiZXhwIjpudWxsLCJ0eXBlIjoiYXJkdWlubyJ9.J703XpO_3hBH3f6LmqKc7sY5uNIsbTGkO_YsrSaqGYU"));
        client.println(F("Connection: close"));
        client.println();
        requestDone = true;
    }
}

char* ConnMgr::loop() {
    // if there are incoming bytes available
    // from the server, read them and print them
    while (requestDone && client.available()) {      
      msg.concat((char) client.read());
    }
    
    // // if the server's disconnected, stop the client
    if (requestDone && !client.connected()) {      
      Serial.println(F("Disconnecting from server..."));
      client.stop();
      Serial.println(F("Disconnected from server..."));
      Serial.println("-------");
      requestDone = false;     

      String body = msg.substring(msg.indexOf("\n\r\n") + 3);     
      int bLength = body.length();
      msg = "";

      char* ret = new char[strlen(bLength) + 1];
      strcpy(ret, body.c_str());

      Serial.println("BODY CONTENT: ");
      Serial.print(ret);
      Serial.println("-----------");

      return ret;//CHANGE THISSSSS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! This should return ret          
    }

    return NULL;
}
