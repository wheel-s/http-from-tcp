#include "request.h"

int main(){

    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;
    char *message="hello";
    char header_buffer[512];
    char buf[1024];

    if(WSAStartup(MAKEWORD(2,2), &wsa) !=0){
        printf("Failed. Error Code %lu\n", WSAGetLastError);
        return 1;
    }
    server_socket =socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port= htons(8000);

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));

    listen(server_socket, 5);
    printf("Waiting for in coming connections.....\n");
    int n;
    while(1){
        c = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
        //
        int total_received = 0;
        buf[0]= '\0';
        reqStatus r = RequestFromSocket(client_socket);

        const char* response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Connection: close\r\n"
            "Content-Length: 12\r\n"
            "\r\n"
            "Hello World!";

        int sent = send(client_socket,response, strlen(response), 0);


        if(sent == SOCKET_ERROR){
            printf("send failed with error: %lu\n", WSAGetLastError());
        }


    }


    return 0;
}
