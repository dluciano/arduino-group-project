#include "Feedback.h" 
 #include <SoftwareSerial.h>
 #include <string.h>

Feedback::Feedback(char* username, char* oper){
    this->username = username;
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
        char *token = strtok_r(fbs[x], ":", &saveptr2);
        char *username = (char*) malloc(strlen(token) + 1);
        strcpy(username, token);
        username[strlen(token)] = '\0';
        char *msg = strtok_r(NULL, ":", &saveptr2);
        feedbacks[x] = new Feedback(username, msg);
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

Feedback::~Feedback() {
  free(username);
}

