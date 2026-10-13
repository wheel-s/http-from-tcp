#include "server.h"
#include <signal.h>

#define port 8000

Server s;

void handle_signal(int signal){
	printf(" %d signal received, closing server and cleaning up...\n", signal);
    Close(&s);
    s.closed = true;
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

char *chunk = "part one of this is Teej"
            "part 2 of this is the Lane"
            "part3 of this is the primeagen"
            "parrt of this is the LOW level";



void handler(SOCKET client_sock, Request_t* req){
    printf("Handling Path: %s\n", req->RequestLine.requestTarget);
    Body body = {
        .data = get_200,
        .len = strlen(get_200)
    };
    Headers_t headers = GetDefaultHeaders(body.len);
    statusCode status = HTTP_STATUS_OK;
    headers_put(&headers, "content-type", "text/html\r\n");
    char val[10];
    snprintf(val, sizeof(val), "%d",body.len);
    headers_put(&headers, "content-length", val);
    if(strcmp(req->RequestLine.requestTarget, "/yourproblem") == 0){ 
        status = HTTP_STATUS_BAD_REQUEST;
        body.data = get_400;
        body.len = strlen(get_400);
        char val[10];
        snprintf(val, sizeof(val), "%d",body.len);
        headers_put(&headers, "content-length", val);
        headers_put(&headers, "content-type", "text/html\r\n");

    }else if(strcmp(req->RequestLine.requestTarget, "/myproblem") == 0){
        status = HTTP_STATUS_INTERNAL_SERVER_ERROR;
        body.data = get_500;
        body.len = strlen(get_500);
        char val[10];
        snprintf(val, sizeof(val), "%d",body.len);
        headers_put(&headers, "content-length", val);
        headers_put(&headers, "content-type", "text/html\r\n");

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
        headers_put(&headers, "content-length", "20183615");
        headers_put(&headers, "content-type", "video/mp4\r\n");
     }else if(strstr(req->RequestLine.requestTarget, "/httpbin/stream") != NULL){
         stringReader read = {
            .data = "parrt of this is the LOW level\npart3 of this is the primeagen\npart 2 of this is the Lane\npart one of this is Teej\n",
            .totalLength = strlen(read.data),
            .pos = 0,
        };
        char dest[5];
        int bytes_read;
        WriteStatusLine(client_sock, status);
        replace_header(&headers, "content-length", "Transfer-Encoding", "chunked");
        headers_put(&headers, "content-cype", "text/plain\r\n");
        WriteHeaders(client_sock, headers); 
        for(;;){
            bytes_read = readChunk(&read, dest, 4);
            dest[bytes_read] = '\0';
            // printf("%s\n",dest);
            // printf("%x\n",bytes_read);
            if(bytes_read == 0){
                break;
            }
            char val[20];
            int hex = snprintf(val, sizeof(val), "%x\r\n", bytes_read);
            send(client_sock, val , hex ,0);
            send(client_sock, dest, bytes_read,0);
            send(client_sock, "\r\n", 2, 0);
        }
            send(client_sock, "0\r\n\r\n", 5 ,0);
        return;
    }

    WriteStatusLine(client_sock, status);
    WriteHeaders(client_sock, headers);
    WriteBody(client_sock, &body);
    return;

}

int main (){

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGABRT, handle_signal);

    s = startServer(port, handler);

    if(s.closed == true){
        printf("Error starting server: -1");
        return -1;
    }
}

