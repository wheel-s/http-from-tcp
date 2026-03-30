#ifndef REQUEST_H
#define REQUEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>






typedef struct Headers{
    char key[256];
    char value[256];

}Headers_t;





typedef struct ChunkReader{
        const char *data;
        int numBytesPerRead;
        int pos;
}ChunkReader_t;


typedef enum {

    INITIALIZED,
    requestStateParsingHeaders,
    parsingbody,
    DONE

}state;


typedef struct RequestLine{
    char HttpVersion[30];
    char RequestTarget[30];
    char Method[30]; 
}RequestLine_t;

typedef struct {
    RequestLine_t RequestLine;
    Headers_t Headers[10];
    void *body;
    int num_headers;
    state state;
}Request_t;



int chunkReaderRead(ChunkReader_t *cr, char *buf, int bufSize);

Request_t* RequestFromReader(ChunkReader_t *cr);
int parse(Request_t *req, char *data, int len);


int Parse(Request_t *req, int index, char *data);
char * Get(Request_t *req, char *string);


typedef struct ChunkReader{
        const char *data;
        int numBytesPerRead;
        int pos;
}ChunkReader_t;







#endif