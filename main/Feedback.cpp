#include "Feedback.h" 
 #include <SoftwareSerial.h>
 #include <string.h>

Feedback::Feedback(int userId,  char* oper){
    this->userId = userId;
    this->oper = oper;
}


Feedback** Feedback::asFeedback(char *body){
    if(strlen(body) <= 0)
        return NULL;

    char* cBody = new char[strlen(body) + 1];
    strcpy(cBody, body);

    int feedbacksCount = 1;

    for(int i = 0; i < strlen(cBody); i++) {
        if(cBody[i] == '|') {
            feedbacksCount++;
        }
    }

    char *saveptr1, *saveptr2;
    char* feedbacksStr = strtok_r(cBody, "|", &saveptr1);
    char** fbs = new char*[feedbacksCount + 1];

    int i = 0;
    while(feedbacksStr != NULL){
        fbs[i] = feedbacksStr;
        feedbacksStr = strtok_r(NULL, "|", &saveptr1);
        i++;
    }
    fbs[feedbacksCount] = NULL;

    Feedback** feedbacks = new Feedback*[feedbacksCount + 1];    
    char *f = NULL;

    for(int x = 0; x < i; x++) {
        f = strtok_r(fbs[x], ":", &saveptr2);
        int userId = atoi(f);
        char *msg = strtok_r(NULL, ":", &saveptr2);
        feedbacks[x] = new Feedback(userId, msg);
    }

    // Delete the fbs variable from memory
    i = 0;
    while(fbs[i] != NULL) {
      fbs[i][0] = '\0';
      i++;
    }
    delete [] fbs;

    // Delete the cBody variable from memory
    delete [] cBody;

    feedbacks[feedbacksCount] = NULL;
    return feedbacks; //feedbacks
}
