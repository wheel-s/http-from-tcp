#pragma once
#include "headers.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


typedef struct RequestLine{
    char* httpVersion;
    char* requestTarget;
    char* method; 
} RequestLine_t;

typedef enum {
    INITIALIZED,
    requestStateParsingHeaders,
    parsingbody,
    DONE
} state;

typedef struct {
    RequestLine_t RequestLine;
    Headers_t Headers;
    int len;
    void* body;
    state state;
} Request_t;


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


typedef struct {
    parseStatus status;
    Request_t *req;
}reqStatus;

typedef struct stringReader {
    char *data;
    int totalLength;
    int pos;
}stringReader;

int readChunk(stringReader *stringReader, char *dest, int toRead);

reqStatus RequestFromSocket(SOCKET client);
reqStatus RequestFromReader(stringReader *read);

parseResult parse(Request_t *req, char *data, int len);
parseResult Parse(Request_t *req, char *data, int len);

void free_req(Request_t *req);
