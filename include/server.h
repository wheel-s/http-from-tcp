#pragma once
#include "request.h"

typedef enum {
    HTTP_STATUS_OK = 200,
    HTTP_STATUS_BAD_REQUEST = 400,
    HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
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


void WriteStatusLine(SOCKET client_sock, statusCode status);
Headers_t GetDefaultHeaders(int contentLen);
int WriteHeaders(SOCKET client_sock, Headers_t headers);

typedef struct {
    void* data;
    int len;
}Body;

void WriteBody(SOCKET client_sock, Body* body);

