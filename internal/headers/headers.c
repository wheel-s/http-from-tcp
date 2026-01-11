#include "../../include/request.h"
#include<stdlib.h>
#include <stdio.h>
#include <string.h>



typedef struct Headers{
    char *key;
    char *value;

}Headers_t;




int Parse(Headers_t *headers,char *data ){

    char *newline = strstr(data,"\r\n");
    
    if(!newline){ return 0;}
    
    if(strncmp(newline+2, "\r\n", 2)==0){
     printf("hello");
        
    }


    int n =  sizeof(data);

    return n;

}



int main(){
        
     
    char *data = "Host: localhost:8080\r\n\r\n";
    Headers_t headers[5];

    Parse(headers, data);



    return 0;

}