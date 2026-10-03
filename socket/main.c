 
#include "../include/request.h"
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void getLinesChannel(SOCKET *client, char* buff){
    int n;
    char str[512] = "";
    char buf[1024] ={0};

    while(1){
        n = recv(*client, buf, 8, 0);
        if(n<= 0){ break; }

        buf[n] = '\0';
        strcat(buff, buf);
        char *newline = strstr(buf,"\n");
        if(strstr(buff,"\r\n\r\n") != NULL){
                break;
        }

        if(newline){ 
            strncat(str, buf, newline - buf);
            printf("read: %s\n", str);
            str[0] = '\0';
            strcat(str, newline + 1);
        }else {
            strcat(str, buf);
        }
    }
    return;
}

int main(){

    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;
    char *message="hello";
    char header_buffer[512];
    char buf[1024];

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
            printf("send failed with error: %d\n", WSAGetLastError());
        }

        closesocket(client_socket);
      //
      //   FILE *file = fopen("./socket/usha.mp4", "rb");
      //       if( file == NULL ){
      //       perror("error opening file");
      //       return -1;
      //   }
      //
      // //file size
      //   fseek(file, 0, SEEK_END);
      //   long file_size  = ftell(file);
      //   fseek(file, 0, SEEK_SET);
      //
      //   long long start = 0;
      //   long long end = file_size - 1;
      //   int is_range = 0;
      //
      //   char * range_header = strstr(buf, "Range: bytes=");
      //   if(range_header) {
      //       is_range = 1;
      //       if(sscanf(range_header, "Range: bytes=%lld-%lld", &start, &end) < 2) {
      //           end = file_size - 1;
      //       }
      //   }
      //
      //   if (start >= file_size) start = 0;
      //   if(end >= file_size ||end < start )end = file_size - 1;
      //   long long content_length = (end - start) + 1;
      //   int header_len = 0;
      //   if(is_range) {
      //       header_len = snprintf(header_buffer, sizeof(header_buffer),
      //                               "HTTP/1.1 206 partial Content\r\n"
      //                               "Content-Type: video/mp4\r\n"
      //                               "Content-Length: %lld\r\n"
      //                               "Content-Range: bytes %lld-%lld/%lld\r\n"
      //                               "Accept-Ranges: bytes\r\n"
      //                               "Connection: close\r\n"
      //                               "\r\n",
      //                               content_length, start, end, file_size
      //                           );
      //   } else {
      //
      //       header_len = snprintf(header_buffer, sizeof(header_buffer),
      //                               "HTTP/1.1 200 OK\r\n"
      //                               "Content-Type: video/mp4\r\n"
      //                               "Content-Length: %lld\r\n"
      //                               "Accept-Ranges: bytes\r\n"
      //                               "Connection: close\r\n"
      //                               "\r\n",
      //                               file_size
      //                           );
      //   }
      //   send(client_socket, header_buffer, header_len, 0);
      //
      //   _fseeki64(file, start, SEEK_SET);
      //
      //   long long remaining = content_length;
      //
      //   unsigned char buffer[8192];
      //   size_t bytes_read;
      //
      //   while(remaining > 0) {
      //       size_t to_read = (remaining < sizeof(buffer)) ? (size_t)remaining : sizeof(buffer);
      //       size_t bytes_read = fread(buffer, 1, to_read, file);
      //
      //       size_t total_sent = 0;
      //       while(total_sent < bytes_read) {
      //           ssize_t sent =  send(client_socket,(char*)buffer + total_sent, (int)( bytes_read -  total_sent), 0);
      //
      //               if(sent <= 0 ) {
      //                   perror("send failed");
      //                   fclose(file);
      //                   return -1;
      //               }
      //               total_sent +=sent;
      //       }
      //       remaining -=bytes_read;
      //   }
      //
      //
      //   if(feof(file)) {
      //       printf("Video successfully transmitted (%ld bytes).\n", file_size);
      //   }
      //   fclose(file);

    }


    return 0;
}
// gcc -Irequest internal/headers/headers.c  internal/request/request.c socket/main.c -o app -lws2_32
