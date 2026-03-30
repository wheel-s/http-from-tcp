#include "../../include/request.h"


int chunkReaderRead(ChunkReader_t *cr, char *buf, int bufSize){
    if(cr->pos >= strlen(cr->data)){
        return 0;
    }

    int remaining = strlen(cr->data) - cr->pos;
    int toRead = cr->numBytesPerRead;
    if(toRead > remaining){
        toRead =remaining;
    }
    if(toRead >bufSize){
        toRead = bufSize;
    }


    memcpy(buf, cr->data + cr->pos, toRead);
    cr->pos+=toRead;

    return toRead;
}






int parseRequestLine(char *reqLine, Request_t *req){
  
    char fullVersion[20];
    if(sscanf(reqLine, "%s %s %s", req->RequestLine.Method, req->RequestLine.RequestTarget, fullVersion)!=3){return -1;};

    
    for(int i=0; req->RequestLine.Method[i]; i++){
         if(!isupper(req->RequestLine.Method[i])){ return -1;}
     }

    if(strcmp(fullVersion, "HTTP/1.1")!=0) {return -1;}
    strcpy(req->RequestLine.HttpVersion, "1.1");   

   
     
}


int parse(Request_t *req, char *data, int len){


    data[len] = '\0';
 
    char *newline = strstr(data,"\r\n");    
    if(!newline){ return 0;}   
     
    int lineLength = newline - data;
        
    if(lineLength>=512){
        return -1;
    }

    char line[512];
    strncpy(line, data, lineLength);   
    line[lineLength] ='\0';
      
  
    if(req->state ==INITIALIZED){

        parseRequestLine(line, req);
        req->state = requestStateParsingHeaders;

        return lineLength + 2;
    }

    else if(req->state==requestStateParsingHeaders){
        if(lineLength == 0){
            req->state =parsingbody;
            return 2;
        }
        int  idx = req->num_headers++;
        Parse(req, idx, line);
        
    }
    if(req->state == parsingbody){
        // printf("%s\n", line);
       char *name = Get(req, "content-length");
       if(name =="nayyy"){
            req->state = DONE;
            printf("hello\n");
       }else{

        req->body = malloc(strlen(line)+1);
        strcpy((char*)req->body, line);
        // printf("%s\n",line);
    
        // printf("")
          
       }
      
    }
    

    return lineLength+2;   
}

char *Get(Request_t *req, char* key){
    
    for (int i =0;i<10 && req->Headers[i].key !=NULL;i++){
        // printf("%s\n", req->Headers[i].key);
        if(strcmp(key, req->Headers[i].key)==0){
            char *name  = req->Headers[i].value;
            return name;
        }
        
    }

    return "nayyy";
    // return "yess";
    // printf("hello");
}

Request_t* RequestFromReader(ChunkReader_t *cr){   
   Request_t *Req = malloc(sizeof(Request_t));
   Req->state = INITIALIZED;
    
   char buffer[1024 +1] ={0};
   int buffer_len = 0;

   
   while(Req->state !=DONE){
   
    char chunk[8];
    int n  = chunkReaderRead(cr, chunk, sizeof(chunk));
    
    if(n<=0 ){
       break;
    }
 

    memcpy(buffer + buffer_len, chunk, n);
    buffer_len +=n;
   
    buffer[buffer_len]='\0';
  
    int consumed = parse(Req, buffer, buffer_len);

    if(consumed > 0){
        int remaining = buffer_len - consumed;
       
        memmove(buffer, buffer +consumed, remaining);
        buffer_len -= consumed;
        buffer[buffer_len]='\0';
      
    }

   }


   return  Req;    
}


// gcc -Irequest headers/headers.c request/request.c request/request_test.c -o main 