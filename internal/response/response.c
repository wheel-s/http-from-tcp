#include "../../include/request.h"



void WriteStatusLine(SOCKET client_sock, statusCode status){
   char* response;
    switch (status){
        case 200:
            response = "HTTP/1.1 200 OK\r\n";
            send(client_sock, response, strlen(response), 0);
            return;
        case 400:
            response = "HTTP/1.1 400 BAD REQUEST\r\n";
            send(client_sock, response, strlen(response), 0);
            return;
        case 500:
            response =  "HTTP/1.1 500 INTERNAL SERVER ERROR\r\n";
            send(client_sock, response, strlen(response), 0);
            return;
        default:
            char* response = "HTTP/1.1  %d\r\n",status;
            send(client_sock, response, strlen(response), 0);
            return;
    }

    return;
}


typedef struct responses {
    char* key;
    char* value;
}responses;

Headers_t GetDefaultHeaders(int contentLen){

    responses resp[] = {
        {
            .key = "Content-Length",
            .value = "0"
        },
        {
            .key = "Connection",
            .value ="close"
        },
        {
            .key = "Content-Type",
            .value = "text/plain\r\n"
        },
    };


    Headers_t headers;
    headers_init(&headers, 3);

    for(int i = 0; i < 3; i++){
        headers.items[i].key = strdup(resp[i].key);
        headers.items[i].value = strdup(resp[i].value);
    }
    return headers;
}

int WriteHeaders(SOCKET client_sock, Headers_t headers){
    char line[512];
    printf("%s\n", headers.items[0].value);
    for(int i = 0; i < 3; i++){
        int len = snprintf(line, sizeof(line), "%s: %s\r\n",
                headers.items[i].key,
                headers.items[i].value);
        printf("%s\n", line);
        send(client_sock, line, len, 0);
    }

    return 0;
}

void WriteBody(SOCKET client_sock, Body *body){
    int sent = send(client_sock, body->data, body->len,0);
    if(sent == SOCKET_ERROR){
        printf("send failed with error: %d\n", WSAGetLastError());
    }
    return;
}

void WriteChunkedBody(){

}

void WriteChunkedBodyDone(){

}


