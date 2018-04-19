#include "HttpResponse.h"
#include "Feedback.h"

HttpResponse::HttpResponse(String response){
    int x = response.indexOf("\n\r") + 3;
    Serial.println(x);
    Serial.println(response.charAt(x));
    //body = response.substring(167); //add 4 char, because from the last \n\r to the body there are 4 char (\n\r\n\r)
    //Serial.println(response.substring(167, 173));
    for(int i = x; i < response.length() - i; i++){
      body.concat(response.charAt(i));  
    }
}

Feedback** HttpResponse::asFeedback(){
  Serial.print("BODY:::: ");
  Serial.println(body);
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
      char *msg = strtok(NULL, ":");  
      Serial.println(msg);  
      feedbacks[x] = new Feedback(userId, msg);      
    }
    //TODO: Delete the fbs variable    
    feedbacks[feedbacksCount]  = NULL;

    return feedbacks;
}
