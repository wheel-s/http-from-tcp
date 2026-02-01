#ifndef REQUEST_H
#define REQUEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>






typedef struct Headers{
    char key[40];
    char value[40];

}Headers_t;





typedef struct ChunkReader{
        const char *data;
        int numBytesPerRead;
        int pos;
}ChunkReader_t;


typedef enum {

    INITIALIZED,
    requestStateParsingHeaders,
    DONE

}state;


typedef struct RequestLine{
    char HttpVersion[30];
    char RequestTarget[30];
    char Method[30]; 
}RequestLine_t;

typedef struct {
    RequestLine_t RequestLine;
    Headers_t Headers[8];
    int num_headers;
    state state;
}Request_t;



int chunkReaderRead(ChunkReader_t *cr, char *buf, int bufSize);

Request_t* RequestFromReader(ChunkReader_t *cr);
int parse(Request_t *req, char *data, int len);


int Parse(Request_t *req, int index, char *data);



typedef struct ChunkReader{
        const char *data;
        int numBytesPerRead;
        int pos;
}ChunkReader_t;







#endif