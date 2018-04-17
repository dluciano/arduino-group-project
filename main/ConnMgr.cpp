#include "ConnMgr.h"
#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include "LCDMgr.h"
SoftwareSerial espSerial(3, 2);
WiFiEspClient client;

using namespace openday;

void ConnMgr::setup(){
    // Start esp wifi and its serial interface
    mLcd.log("Configuring WIFI");
    espSerial.begin(9600);
    WiFi.init(&espSerial);  
    connect();
    mLcd.log("WIFI Configured");
}

void ConnMgr::connect(){    
    mLcd.log("Connecting to", "WiFi...", false);    
    if (WiFi.status() != WL_NO_SHIELD) {      
      while (WiFi.status() != WL_CONNECTED) {
        //WiFi.begin("LuisSaul-HS", "pufx1142");
        WiFi.begin("StudentCom", "");
      }
    }
    if(WiFi.status() == WL_CONNECTED)
      mLcd.log("Ready. Waiting", "", false);
    connected = WiFi.status() == WL_CONNECTED;
}

void ConnMgr::get(String server, String path, int port){
    if(WiFi.status() != WL_CONNECTED){
      connected = false;
      mLcd.log("Reconnect Wifi!!", "", false);
      connect();
      return;
    }
    
    if (!requestDone && client.connect(server.c_str(), port)) {
        Serial.println("Connected to server");
        
        String getStr = String("GET " + path + " HTTP/1.1");
        String host =  String("Host: " + server);        
        
        Serial.println("\nGET REQUEST: ");
        Serial.println(getStr);
        
        Serial.println("\HOST REQUEST: ");
        Serial.println(host);

        client.println(getStr);
        client.println(host);
        client.println("Connection: close");
        client.println();
        requestDone = true;
    }
}  

String ConnMgr::loop(){
    // if there are incoming bytes available
    // from the server, read them and print them
    while (requestDone && client.available()) {      
      msg.concat((char) client.read());
    } 
    // // if the server's disconnected, stop the client
    if (requestDone && !client.connected()) {
      Serial.println();
      Serial.println("Disconnecting from server...");
      client.stop();
      Serial.println("Disconnected from server...");
      requestDone = false;      
      Serial.print("\nMessage:\n");
      Serial.println(msg);
      String aux = String(msg);
      msg = "";
      return aux;
    }
    return "";
}
