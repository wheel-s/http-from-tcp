#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>




typedef enum {
    starting,
    listenning,
    shuttingdown
} serverState;

typedef enum {
    accepted,
    recieving,
    requestComplete,
    sendingResponse,
    closing
}connectionState;

typedef enum {
    preparing,
    sendingStatusLine,
    sendingBody,
    done
} responseState;



typedef struct Header {
    char* key;
    char* value;
}Header;

typedef struct Headers {
    Header* items;
    int capacity;
    int size;
}Headers_t;

bool is_valid_header(char* field);
int headers_getkey(Headers_t* headers, char* key);
Headers_t* headers_init(Headers_t *headers, int capacity);
void headers_put(Headers_t *headers, int index, char* key, char* value);
Header headers_get(Headers_t *headers, int index);
void headers_delete(Headers_t *headers, int index);
void headers_push(Headers_t *headers, char*key, char* value);
void resize_headers(Headers_t *header, int capacity);

typedef enum {
    INITIALIZED,
    requestStateParsingHeaders,
    parsingbody,
    DONE
} state;


typedef enum {
    PARSE_OK,

    //stream err
    ERR_INCOMPLETE,
    ERR_LINE_TOO_LONG,

    //RequestLine err
    ERR_BAD_REQUESTLINE,
    ERR_INVALID_METHOD,
    ERR_INVALID_PATH,
    ERR_UNSUPPORTED_VERSION,

    //Header errors
    ERR_BAD_HEADER_LINE,
    HEADERS_DONE_PARSING,
    ERR_INVALID_FIELDNAME,
    ERR_INVALID_CHAR_IN_FIELDNAME,

    //body
    ERR_BAD_BODY,

    // done err
    TRYING_TO_READ_DATA_IN_A_DONE_STATE,
    UNKNOWN_STATE,
    //Memory /fatal
    ERR_NULL_POINTER
} parseStatus;

const char* parse_error_str(parseStatus status);

typedef struct parseReult {
    int read;
    parseStatus status;
} parseResult;

typedef struct RequestLine{
    char* httpVersion;
    char* requestTarget;
    char* method; 
} RequestLine_t;

typedef struct {
    RequestLine_t RequestLine;
    Headers_t Headers;
    void* body;
    state state;
} Request_t;


typedef struct {
    parseStatus status;
    Request_t *req;
}reqStatus;


// int chunkReaderRead(ChunkReader_t *cr, char *buf, int bufSize);
typedef struct stringReader {
    char *data;
    int totalLength;
    int pos;
}stringReader;

reqStatus RequestFromSocket(SOCKET client);
reqStatus RequestFromReader(stringReader *read);

parseResult parse(Request_t *req, char *data, int len);


parseResult Parse(Request_t *req, int index, char *data);
char * Get(Request_t *req, char *string);



int readChunk(stringReader *stringReader, char *dest, int toRead);

typedef enum {
    HTTP_STATUS_200 = 200,
    HTTP_STATUS_400 = 400,
    HTTP_STATUS_500 = 500,
}statusCode;



typedef struct {
    char* message;
    statusCode status;
} HandleError;

typedef void (*Handler)(SOCKET client_sock, Request_t* req);

typedef struct Server {
    SOCKET server_socket;
    bool closed;
    Handler handler;
} Server;

Server startServer(int port, Handler handler);
int Close(Server* s);
void Listen(Server* s);
void handle(Server* s, SOCKET client_sock);

void freeRequest(Request_t* req);



void WriteStatusLine(SOCKET client_sock, statusCode status);
Headers_t GetDefaultHeaders(int contentLen);
int WriteHeaders(SOCKET client_sock, Headers_t headers);

typedef struct {
    void* data;
    int len;
}Body;

void WriteBody(SOCKET client_sock, Body* body);
