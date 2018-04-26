#ifndef Feedback_h
#define Feedback_h
#endif

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class Feedback{
  public:
  Feedback(char* username,  char* oper);

  static Feedback** asFeedback(char *body);

  char* username;
  char* oper;

  ~Feedback();
};
