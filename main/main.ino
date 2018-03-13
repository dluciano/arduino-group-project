#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <string.h>

SoftwareSerial espSerial(3, 2);
WiFiEspClient client;

class Feedback{
  public:
  Feedback(int userId,  int oper){
    this->userId = userId;
    this->oper = oper;
  }

  int userId;
  int oper;
};

struct HttpResponse {
  construct(String response){
    int x = response.indexOf("\n\r") - 1;   
    body = response.substring(x+4); //add 4 char, because from the last \n\r to the body there are 4 char (\n\r\n\r)
  }
  String body;

  Feedback** asFeedback(){
    if(body == "")
      return NULL;
    
    int feedbacksCount = 1;    
    for(int i = 0; i < body.length(); i++){      
      if(body.charAt(i) == '|'){
        feedbacksCount++;       
      }
    }    

    char* feedbacksStr = strtok(body.c_str(), "|");    
    char** fbs = new char*[feedbacksCount];
    int i = 0;
    while(feedbacksStr != NULL){
      fbs[i] = feedbacksStr;
      feedbacksStr = strtok(NULL, "|");
      i++;
    }
    
    Feedback** feedbacks = new Feedback*[feedbacksCount + 1];    
    char *f = NULL;
    for(int x = 0; x < i; x++){      
      f = strtok(fbs[x], ":");
      int userId = atoi(f);
      f = strtok(NULL, ":");
      int oper = atoi(f);
      feedbacks[x] = new Feedback(userId, oper);      
    }
    //TODO: Delete the fbs variable    
    feedbacks[feedbacksCount]  = NULL;

    return feedbacks;
  }
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
        WiFi.begin("LuisSaul-HS", "pufx1142");
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
 Serial.println("Configuring WIFI");
 conn.setup();
 Serial.println("WIFI Configured");

 Serial.println("Configuring Sensors and Components");
 op.setup();
 Serial.println("All Ready!");
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
    HttpResponse response;
    response.construct(s);    
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
