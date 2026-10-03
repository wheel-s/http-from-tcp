#include <stdlib.h>
#include "../include/request.h"
#include <stdio.h>
#include <signal.h>
#include <windows.h>

#define port 8000

void handle_signal(int signal){
	printf(" %d signal received, Cleaning up...\n", signal);
	exit(EXIT_SUCCESS);
}

char* get_400 = "<html>"
            "<head>"
                "<title>400 Bad Request</title>"
            "</head>"
            "<body>"
                "<h1>Bad Request</h1>"
                "<p>Your request honestly kinda sucked.</p>"
            "<body>"
        "</html>";
char* get_200 = "<html>"
        "<head>"
            "<title>200 OK</title>"
        "</head>"
        "<body>"
            "<h1>Success!</h1>"
            "<p>Your request was an absolute banger.</p>"
        "<body>"
    "</html>";

char* get_500 = "<html>"
            "<head>"
                "<title>500 Internal Server Error</title>"
            "</head>"
            "<body>"
                "<h1>Internal Server Error</h1>"
                "<p>Okay, you know what? This one is on me.</p>"
            "<body>"
        "</html>";



void handler(SOCKET client_sock, Request_t* req){
    printf("Handling Error......%s\n", req->RequestLine.requestTarget);
    Body body = {
        .data = get_200,
        .len = strlen(get_200)
    };
    Headers_t headers = GetDefaultHeaders(body.len);
    statusCode status = HTTP_STATUS_200;
    headers.items[2].value = "text/html\r\n";
    char val[10];
    snprintf(val, sizeof(val), "%d",body.len);
    headers.items[0].value = val;
    if(strcmp(req->RequestLine.requestTarget, "/yourproblem") == 0){ 
        body.data = get_400;
        body.len = strlen(get_400);
        char val[10];
        snprintf(val, sizeof(val), "%d",body.len);
        headers.items[0].value = val;
        headers.items[2].value = "text/html\r\n";

    }else if(strcmp(req->RequestLine.requestTarget, "/myproblem") == 0){
        body.data = get_500;
        body.len = strlen(get_500);
        char val[10];
        snprintf(val, sizeof(val), "%d",body.len);
        headers.items[0].value = val;
        headers.items[2].value = "text/html\r\n";

    }else if(strcmp(req->RequestLine.requestTarget, "/video") == 0){

        FILE *f = fopen("./assets/vim.mp4", "rb");
        if( f == NULL){ 
            printf("no such file\n");
            return;
        }
        body.data = malloc(20183615);
        body.len = 20183615;
        fread(body.data, 1, 20183615, f);
        fclose(f);
        char val[10];
        snprintf(val, sizeof(val), "20183615",body.len);
        headers.items[0].value = "20183615";
        headers.items[2].value = "video/mp4\r\n";


     }
    // elseif(strstr(req->RequestLine, "/httpbin/stream") != NULL){
    //
    // }

    WriteStatusLine(client_sock, status);
    WriteHeaders(client_sock, headers);
    WriteBody(client_sock, &body);
    
    return;

}

int main (){

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGABRT, handle_signal);

    Server s = startServer(port, handler);

    if(s.closed == true){
        printf("Error starting server: %d", s);
        return -1;
    }
}

// gcc httpserver/main.c -o http
// gcc -Irequest internal/headers/headers.c  internal/request/request.c internal/server/server.c internal/response/response.c httpserver/main.c -o http -lws2_32
