#include "../../include/request.h"
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>





int Parse(Request_t *req,int index, char *data ){

    // char *newline = strstr(data,"\r\n");
    
    // if(!newline){ return 0;}
    

    // if(strncmp(newline+2, "\r\n", 2)==0){
    //     // printf("last field header ");
    // }


    // if(strpbrk(data, "@!#%^&'*+_`|~")){
    //     printf("invalid characater in header\n");
    //     return 1;
    // }
    
    
   


    char key[40] = {0};
    char value[40]={0};
    sscanf(data,"%s %s",key, value );

    char MainKey[30]={0};
    sscanf(key, "%[^:]",MainKey);
    if(strchr(MainKey, ' ')){
        printf("invalid header\n");
        return 1;
    }


    for (int i = 0; MainKey[i]!='\0'; i++){
        MainKey[i] = tolower((unsigned char) MainKey[i]);  
    }
    
    int update = -1;
    int idx = index + 1;
    for(int i=0; i<idx; i++){
        if(strcmp(req->Headers[i].key, MainKey) ==0 ){
      
            update = i;
            break;
        }
    }
 
    
     
    if( update != -1){
        strcat(req->Headers[update].value, value);  
       
    }else{
        strcpy(req->Headers[index].key, MainKey) ;
        strcpy(req->Headers[index].value, value);
    
    }

   

   
    
    int n =  sizeof(data);


    return n;

}



