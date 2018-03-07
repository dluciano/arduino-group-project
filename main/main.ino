#include <SoftwareSerial.h>
#include <WiFiEsp.h>

SoftwareSerial espSerial(3, 2);
WiFiEspClient client;

struct HttpResponse {
  construct(String response){
    int x = response.indexOf("\n\r") - 1;   
    body = response.substring(x+4); //add 4 char, because from the last \n\r to the body there are 4 char (\n\r\n\r)
  }
  String body;
};

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

  String loop(){
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

  bool connected = false;
  bool requestDone = false;
  String msg;
};

#define LED_PORT 13
struct OperMgr{
  void setup(){
    pinMode(LED_PORT, HIGH);
  }
  void loop(HttpResponse message){
    // Serial.print("\n\nHTTP RESPONSE BODY:<<<: ");
    // Serial.print(message.body.c_str());    
    // Serial.println(" :>>>");
    if(message.body == "1"){
      digitalWrite(LED_PORT, HIGH);
    }else{
      digitalWrite(LED_PORT, LOW);
    }
  }
};

ConnMgr conn;
OperMgr op;

void setup() { 
 Serial.begin(9600);
 Serial.println("Configuring WIFI");
 conn.setup();
 Serial.println("WIFI Configured");

 Serial.println("Configuring Sensors and Components");
 op.setup();
 Serial.println("All Ready!");
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
  String s = conn.loop();
  if(s!= ""){
    HttpResponse response;
    response.construct(s);
    op.loop(response);
  }
}
