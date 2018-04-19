#include "Feedback.h" 
 #include <SoftwareSerial.h>
 #include <string.h>

Feedback::Feedback(int userId,  char* oper){
    this->userId = userId;
    this->oper = oper;
}


Feedback** Feedback::asFeedback(String response){
    if(response == "")
        return NULL;

    int feedbacksCount = 1;

    String body = String("SOBRENATURAL PROGRAMMING"); 
    //int x = body.indexOf("\n\r") - 1;  
    //int index = response - strchr(response, '\r');
    // Serial.println(response[index + 1]);
    //body = body.substring(x+4); //add 4 char, because from the last \n\r to the body there are 4 char (\n\r\n\r)

    // for(int i = 0; i < body.length(); i++){      
    //     if(body.charAt(i) == '|'){
    //         feedbacksCount++;       
    //     }
    // }    
    Serial.println("\n\n\nOK 1");
    Serial.println(response);
    Serial.println("\n\n\n");


    Serial.println("\n\n\nOK 22222->");
    Serial.println(body);
    Serial.println("\n\n\n");
    //char* feedbacksStr = strtok(body.c_str(), "|"); 
    //char** fbs = new char*[feedbacksCount];
    
    // int i = 0;
    // while(feedbacksStr != NULL){
    //     fbs[i] = feedbacksStr;
    //     feedbacksStr = strtok(NULL, "|");
    //     i++;
    // }

    //Feedback** feedbacks = new Feedback*[feedbacksCount + 1];    
    // char *f = NULL;

    // for(int x = 0; x < i; x++){      
    //     f = strtok(fbs[x], ":");
    //     int userId = atoi(f);
    //     char *msg = strtok(NULL, ":");          
    //     feedbacks[x] = new Feedback(userId, msg);      
    // }
    // //TODO: Delete the fbs variable    
    // feedbacks[feedbacksCount]  = NULL;

    return NULL; //feedbacks
}
