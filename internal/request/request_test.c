#include "../../include/request.h"







int main(){

    /* 
        Parsing request line
    */

    // Good Request Line
    // ChunkReader_t cr ={
    //     .data ="GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/8.16.0\r\n\r\n",
    //     .pos=0,
    //     .numBytesPerRead=5
    // };

    // Request_t *r1 = RequestFromReader(&cr);

    // assert(strcmp("GET",r1->RequestLine.Method )==0);
    // assert(strcmp("/",r1->RequestLine.RequestTarget )==0);
    // assert(strcmp("1.1",r1->RequestLine.HttpVersion)==0);
    // char write[100];
    // sprintf(write,  "- Method: %s\n- Target: %s\n- Version: %s\n",r1->RequestLine.Method,r1->RequestLine.RequestTarget, r1->RequestLine.HttpVersion);
    // printf(write);
    // FILE *file =fopen("requestline.txt", "w");
    // fwrite(write, 1,strlen(write), file);
    // fclose(file);

   
//     // Good Request line with path
//     ChunkReader_t cr2 ={
//         .data ="GET /coffee HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/8.16.0\r\n\r\n",
//         .pos=0,
//         .numBytesPerRead=5
//     };

//     Request_t *r2 = RequestFromReader(&cr2);

//     assert(strcmp("GET",r2->RequestLine.Method )==0);
//     assert(strcmp("/coffee",r2->RequestLine.RequestTarget )==0);
//     assert(strcmp("1.1",r2->RequestLine.HttpVersion)==0);

    
//     // Good POST Request line with path
//     ChunkReader_t cr3 ={
//         .data ="GET /coffee HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/8.16.0\r\nAccept: */*\r\nContent-Type: application/json\r\nContent-Length: 20\r\n\r\n'{flavor:dark mode}'\r\n",
//         .pos=0,
//         .numBytesPerRead=5
//     };

//     Request_t *r3 = RequestFromReader(&cr3);

//     assert(strcmp("GET",r3->RequestLine.Method )==0);
//     assert(strcmp("/coffee",r3->RequestLine.RequestTarget )==0);
//     assert(strcmp("1.1",r3->RequestLine.HttpVersion)==0);


      
//     // Invalid Method (Out of order) Request line
//     ChunkReader_t cr4 ={
//         .data ="GET /coffee HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/8.16.0\r\n\r\n",
//         .pos=0,
//         .numBytesPerRead=5
//     };

//     Request_t *r4 = RequestFromReader(&cr4);

//     assert(strcmp("GET",r4->RequestLine.Method )==0);
//     assert(strcmp("/coffee",r4->RequestLine.RequestTarget )==0);
//     assert(strcmp("1.1",r4->RequestLine.HttpVersion)==0);

    



//     /*
//         parsing headers
//     */

// // Test: Standard headers
//     ChunkReader_t cr5 ={
//         .data ="GET /coffee HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/8.16.0\r\nAccept: */*\r\nContent-Type: application/json\r\nContent-Length: 20\r\n\r\n'{flavor:dark mode}'\r\n",
//         .pos=0,
//         .numBytesPerRead=5
//     };
  
//     Request_t *r5 = RequestFromReader(&cr3);

//     assert(strcmp("Host", r5->Headers[0].key )==0);
//     assert(strcmp("/coffee",r5->RequestLine.RequestTarget )==0);
//     assert(strcmp("1.1",r5->RequestLine.HttpVersion)==0);


// Empty Headers
    // ChunkReader_t cr6 ={
    //     .data ="GET /coffee HTTP/1.1\r\nHost: localhost: 8080\r\nUser-Agent: curl/8.16.0\r\nAccept: */*\r\nContent-Type: application/json\r\nContent-Length: 20\r\n\r\n'{flavor:light mode}'\r\n",
    //     .pos=0,
    //     .numBytesPerRead=5
    // };
  
    // Request_t *r6 = RequestFromReader(&cr6);
       
    // printf("%s\n%s\n", r6->Headers[2].key, r6->Headers[2].value);
    // assert(strcmp("host", r6->Headers[0].key )==0);
    // assert(strcmp("/coffee",r6->RequestLine.RequestTarget )==0);
    // assert(strcmp("1.1",r6->RequestLine.HttpVersion)==0);

    

// other edge cases
/*
malformed headers
Duplicate headers 
Case insensitive Heeaders
Missing End of Headers

*/








//Test body

    ChunkReader_t cr6 ={
        .data ="GET /coffee HTTP/1.1\r\nHost: localhost: 8080\r\nUser-Agent: curl/8.16.0\r\nAccept: */*\r\nContent-Type: application/json\r\nContent-Length: 20\r\n\r\n'{flavor:light mode, name:wheel-s}'\r\n",
        .pos=0,
        .numBytesPerRead=5
    };
  
    Request_t *r6 = RequestFromReader(&cr6);
       
    printf("%s", (char*)r6->body);
    assert(strcmp("host", r6->Headers[0].key )==0);
    assert(strcmp("/coffee",r6->RequestLine.RequestTarget )==0);
    assert(strcmp("1.1",r6->RequestLine.HttpVersion)==0);

    
// other testcases like cchecking content-length or ......








    return 0;
}