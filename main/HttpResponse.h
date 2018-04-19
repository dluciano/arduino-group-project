#ifndef HttpResponse_h
#define HttpResponse_h
#endif

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

typedef class Feedback;

class HttpResponse {
  public:
  HttpResponse(String response);
  
  Feedback** asFeedback();

  private:
  String body;
};
