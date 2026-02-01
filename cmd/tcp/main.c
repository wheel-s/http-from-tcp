#include "../../include/request.h"
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


int main(){
    
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;
    char *message="hello";
    
    if(WSAStartup(MAKEWORD(2,2), &wsa) !=0){
        printf("Failed. Error Code %d\n", WSAGetLastError);
        return 1;
    }
    server_socket =socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port= htons(8080);

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));

    listen(server_socket, 5);
    printf("Waiting for in coming connections.....\n");

    while(1){
        c = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
        char buf[1024];
        int bytes_recieved = recv(client_socket, buf, sizeof(buf)-1, 0);
        buf[bytes_recieved] ='\0';

        
        ChunkReader_t cr ={
            .data =buf,
            .pos=0,
            .numBytesPerRead=5
        };

       
    Request_t *r6 = RequestFromReader(&cr);
       
    printf("%s\n", r6->Headers[1].value);
       
     
        printf("Connection accepted! \n");
    
        char *msg = 
         "HTTP/1.1 200 OK\r\n"
         "Content-Length:5\r\n"
         "\r\n"
         "hello";
        
         send(client_socket,msg, strlen(msg), 0);

    }


    return 0;
}

//gcc -Irequest cmd/tcp/main.c internal/request/request.c -o app -lws2_32 