#ifndef ConnMgr_h
#define ConnMgr_h
#endif

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class ConnMgr {  
  public:
  void setup();
  void connect();
  void get(char *server, char *path, int port = 80);
  char* loop();

  private:
  bool connected = false;
  bool requestDone = false;  

  String msg = String("");
};