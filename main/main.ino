#include <SoftwareSerial.h>
#include <WiFiEsp.h>

#define ESP_RX 3
#define ESP_TX 2
#define WIFI_PARAMS "StudentCom", ""
char server[] = "134.220.4.152";

SoftwareSerial espSerial(ESP_RX, ESP_TX);
WiFiEspClient client; // WiFi client to be able to create TCP connections


struct ConnMgr {
  void setup(){}
};

ConnMgr conn;

void printMac(){
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}

void setup() {
 // Start serial
 Serial.begin(9600);

 // Start esp wifi and its serial interface
 espSerial.begin(9600);
 WiFi.init(&espSerial);

 // Check for presence of WiFi module
 if (WiFi.status() != WL_NO_SHIELD) {
   // If WiFi module present, hang until connected to specified ssid
   while (WiFi.status() != WL_CONNECTED) {
     WiFi.begin(WIFI_PARAMS);
   }
 }

  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    client.println("GET /~1720203/test.php HTTP/1.1");
    client.println("Host: 134.220.4.152");
    client.println("Connection: close");
    client.println();
  }
  printMac();
}


bool dis = false;
void loop() {
 // put your main code here, to run repeatedly:
 // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  } 
  // if the server's disconnected, stop the client
  if (!client.connected() && !dis) {
    dis = true;
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();    
    Serial.println("Disconnected from server...");
  }
}
