#include <SoftwareSerial.h>
#include <WiFiEsp.h>

SoftwareSerial espSerial(3, 2);
WiFiEspClient client;

struct ConnMgr {
  void setup(){
    // Start esp wifi and its serial interface
    espSerial.begin(9600);
    WiFi.init(&espSerial);  
    connect();
  }

  void connect(){
    Serial.println("Connecting to WiFi");
    if (WiFi.status() != WL_NO_SHIELD) {      
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin("StudentCom", "");
      }
    }
    connected = WiFi.status() == WL_CONNECTED;
  }

  void get(String server, String path, int port = 80){
    if(!connected){
      Serial.println("The wifi is not connected. Trying to connect to wifi");
      connect();
      return;
    }    
    
    if (!requestDone && client.connect(server.c_str(), port)) {
        Serial.println("Connected to server");
        
        String getStr = String("GET " + path + " HTTP/1.1");
        String host =  String("Host: " + server);        
        
        Serial.print("\nGET REQUEST: ");
        Serial.println(getStr);
        
        Serial.print("\HOST REQUEST: ");
        Serial.println(host);

        client.println(getStr);
        client.println(host);
        client.println("Connection: close");
        client.println();
        requestDone = true;
    }
  }  

  void loop(){
    // if there are incoming bytes available
    // from the server, read them and print them
    while (requestDone && client.available()) {
      char c = client.read();
      Serial.write(c);
    } 
    // if the server's disconnected, stop the client
    if (requestDone && !client.connected()) {
      Serial.println();
      Serial.println("Disconnecting from server...");
      client.stop();    
      Serial.println("Disconnected from server...");
      requestDone = false;
    }
  }

  bool connected = false;
  bool requestDone = false;
};

ConnMgr conn;

void setup() { 
 Serial.begin(9600);
 conn.setup();
}

#define  REFRESH_RATE 5000
unsigned long long last = 0;
unsigned long long prev = -REFRESH_RATE;

void loop() {
  last = millis();
  if(last - prev >= REFRESH_RATE){
    prev = last;
    conn.get("134.220.4.152", "/~1720203/test.php");  
  }
  conn.loop();
}
