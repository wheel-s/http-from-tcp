#include "../include/request.h"







int main(){

    // Good Request Line
    ChunkReader_t cr ={
        .data ="GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/8.16.0\r\n\r\n",
        .pos=0,
        .numBytesPerRead=5
    };

    Request_t *r1 = RequestFromReader(&cr);

    assert(strcmp("GET",r1->RequestLine.Method )==0);
    assert(strcmp("/",r1->RequestLine.RequestTarget )==0);
    assert(strcmp("1.1",r1->RequestLine.HttpVersion)==0);
    char write[100];
    sprintf(write,  "- Method: %s\n- Target: %s\n- Version: %s\n",r1->RequestLine.Method,r1->RequestLine.RequestTarget, r1->RequestLine.HttpVersion);
    printf(write);
    FILE *file =fopen("requestline.txt", "w");
    fwrite(write, 1,strlen(write), file);
    fclose(file);

   
    // Good Request line with path
    ChunkReader_t cr2 ={
        .data ="GET /coffee HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/8.16.0\r\n\r\n",
        .pos=0,
        .numBytesPerRead=5
    };

    Request_t *r2 = RequestFromReader(&cr2);

    assert(strcmp("GET",r2->RequestLine.Method )==0);
    assert(strcmp("/coffee",r2->RequestLine.RequestTarget )==0);
    assert(strcmp("1.1",r2->RequestLine.HttpVersion)==0);

    
    // Good POST Request line with path
    ChunkReader_t cr3 ={
        .data ="GET /coffee HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/8.16.0\r\nAccept: */*\r\nContent-Type: application/json\r\nContent-Length: 20\r\n\r\n'{flavor:dark mode}'\r\n",
        .pos=0,
        .numBytesPerRead=5
    };

    Request_t *r3 = RequestFromReader(&cr3);

    assert(strcmp("GET",r3->RequestLine.Method )==0);
    assert(strcmp("/coffee",r3->RequestLine.RequestTarget )==0);
    assert(strcmp("1.1",r3->RequestLine.HttpVersion)==0);


      
    // Invalid Method (Out of order) Request line
    ChunkReader_t cr4 ={
        .data ="GET /coffee HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/8.16.0\r\n\r\n",
        .pos=0,
        .numBytesPerRead=5
    };

    Request_t *r4 = RequestFromReader(&cr4);

    assert(strcmp("GET",r4->RequestLine.Method )==0);
    assert(strcmp("/coffee",r4->RequestLine.RequestTarget )==0);
    assert(strcmp("1.1",r4->RequestLine.HttpVersion)==0);

    

    











// gcc request_test.c request.c -o request_test
    return 0;
}