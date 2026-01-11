#ifndef REQUEST_H
#define REQUEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>







typedef struct {
        const char *data;
        int numBytesPerRead;
        int pos;
}ChunkReader_t;


typedef enum {

    INITIALIZED,
    DONE

}state;


typedef struct {
    char HttpVersion[30];
    char RequestTarget[30];
    char Method[30]; 
}RequestLine_t;

typedef struct {
    RequestLine_t RequestLine;
    state state;
}Request_t;



int chunkReaderRead(ChunkReader_t *cr, char *buf, int bufSize);

Request_t* RequestFromReader(ChunkReader_t *cr);
int parse(Request_t *req, char *data, int len);



typedef struct {
        const char *data;
        int numBytesPerRead;
        int pos;
}ChunkReader_t;

#endif