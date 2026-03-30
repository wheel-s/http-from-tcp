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
    server.sin_port= htons(8000);

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
     printf("Connection accepted! \n");
    printf("Request line\n");
    printf( "- Method: %s\n- Target: %s\n- Version: %s\n",r6->RequestLine.Method,r6->RequestLine.RequestTarget, r6->RequestLine.HttpVersion);
    // char write[100];
  
    // printf("%s\n%s\n", r6->Headers->key[0], r6->Headers->value[0]);
  
    printf("Headers:\n");
    int offset =0;
    char write[2024];
    for (int i =0;i<10 && r6->Headers[i].key !=NULL;i++){
        if(strlen(r6->Headers[i].key) >0){ 
            offset+= sprintf(write + offset,"- %s: %s\n", r6->Headers[i].key, r6->Headers[i].value);
        }
    }

    printf(write);
    char *path = "tmp/headers.txt";
    FILE *file =fopen(path, "w");
    fwrite(write, 1,strlen(write), file);
    fclose(file);
    

    char write2[2024];
    printf("...\n");
    sprintf(write2,  "Headers:\n- Method: %s\n- Target: %s\n- Version: %s\n",r6->RequestLine.Method,r6->RequestLine.RequestTarget, r6->RequestLine.HttpVersion);
    printf(write2);
    char *path2 = "tmp/requestline.txt";
    FILE *file2 =fopen(path2, "w");
    fwrite(write2, 1,strlen(write2), file2);
    fclose(file2);

    char write3[2024];
    printf("...\n");
    sprintf(write3,  "- BODY:\n %s\n",r6->body);
    printf(write3);
    char *path3 = "tmp/body.txt";
    FILE *file3 =fopen(path3, "w");
    fwrite(write3, 1,strlen(write3), file3);
    fclose(file3);

    
    
        char *msg = 
         "HTTP/1.1 200 OK\r\n"
         "Content-Length:5\r\n"
         "\r\n"
         "hello";
        
         send(client_socket,msg, strlen(msg), 0);

    }


    return 0;
}

//gcc -Irequest cmd/tcp/main.c internal/request/request.c internal/headers/headers.c -o app -lws2_32 