#include "server.h"

Server startServer(int port, Handler handler){

    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server;


    if(WSAStartup(MAKEWORD(2,2), &wsa) !=0){
        printf("Failed. Error Code %d\n", WSAGetLastError());
        return (Server) { server_socket,true};
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));

    listen(server_socket, 5);
    printf("Waiting for connections........\n");

    Server s = {
        .server_socket = server_socket,
        .handler  = handler,
        .closed = false
    };
    Listen(&s);
    return s;

}


int Close(Server* s){
    if(!s){ return -1; }

    closesocket(s->server_socket);
    s->closed = true;

    return 0;
}

void Listen(Server* s){
    if(!s){ return; }

    SOCKET client_sock; 
    struct sockaddr_in client;
    int c;

    while(1){
        client_sock = accept(s->server_socket, (struct sockaddr *)&client_sock, &c);
        if(client_sock == INVALID_SOCKET){
            if(s->closed == true){
                return;
            }
            printf("Error accepting new connection: %d", WSAGetLastError());
            continue;
        }

        handle(s, client_sock);

   }
}

void free_req(Request_t *req){

    free(req->RequestLine.method);
    free(req->RequestLine.requestTarget);
    free(req->RequestLine.httpVersion);


    for( int i = 0; i < req->Headers.size; i++) {
        free(req->Headers.items[i].key);
        free(req->Headers.items[i].value);
    }
    free(req->Headers.items);

    free(req->body);

    free(req);
    return;
}


void handle(Server *s, SOCKET client_sock){
    if(!s || !client_sock){
        Close(s);
        return;
    }

    Headers_t headers = GetDefaultHeaders(13);
    reqStatus r = RequestFromSocket(client_sock);
    const char* stat = parse_error_str(r.status);
    printf("%s\n", stat);

    if(r.status != PARSE_OK){
        WriteStatusLine(client_sock, HTTP_STATUS_BAD_REQUEST);
        WriteHeaders(client_sock, headers);
        free_req(r.req);
        return;
    }

    s->handler(client_sock, r.req);
    if(r.req != NULL){
        free_req(r.req);
    }
}
