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












RequestLine_t parseRequestLine(char *reqLine){
    RequestLine_t r;
    char RequestLine[50];
    sscanf(reqLine,"%[^\r\n]", RequestLine);

    sscanf(RequestLine,"%s %s HTTP/%s",r.Method, r.RequestTarget, r.HttpVersion);


    return r;

}


int parse(Request_t *req, char *data, int len){
    
    char *newline = strstr(data,"\r\n");
  
    if(!newline){ return 0;}
  
    int lineLength = newline - data;
    char line[512];
    strncpy(line, data, lineLength);
    line[lineLength] ='\0';
   
    char fullVersion[20];
    if(sscanf(line, "%s %s %s", req->RequestLine.Method, req->RequestLine.RequestTarget, fullVersion)!=3){return -1;};

 

    for(int i=0; req->RequestLine.Method[i]; i++){
         if(!isupper(req->RequestLine.Method[i])){ return -1;}
     }

    if(strcmp(fullVersion, "HTTP/1.1")!=0) {return -1;}
    strcpy(req->RequestLine.HttpVersion, "1.1");   
   
    req->state =DONE;
    return lineLength +2;

}

Request_t* RequestFromReader(ChunkReader_t *cr){   
   Request_t *Req = malloc(sizeof(Request_t));
   Req->state = INITIALIZED;
    
   
   char buffer[1024] ={0};
   int buffer_len = 0;

   while(Req->state !=DONE){
    char chunk[8];
    int n  = chunkReaderRead(cr, chunk, sizeof(chunk));
    
    if(n==0 ){break;}
 

    memcpy(buffer + buffer_len, chunk, n);
    buffer_len +=n;
 
    int consumed = parse(Req, buffer, buffer_len);

    if(consumed>0){
        int remaining = buffer_len - consumed;
       
        memmove(buffer, buffer +consumed, remaining);
        buffer_len = remaining;
        
      
        
    }
    else if(consumed < 0){
        free(Req);
        return NULL;
    }

   }

   return  Req;

    
}

