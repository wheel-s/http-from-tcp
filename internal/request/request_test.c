#include "../../include/request.h"
#include <munit.h>
#include <assert.h>
#include <stdbool.h>


typedef struct testCase {
    char*   name;
    char *request;
    char*   wantMethod;
    char*   wantTarget;
    char*   wantVersion;
    parseStatus wantErr;
} testCase;


testCase testCases[] = {
    {
        .name = "Test good requestline",
        .request = "GET / HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n", 
        .wantMethod = "GET",
        .wantVersion = "HTTP/1.1",
        .wantTarget = "/",
        .wantErr = PARSE_OK
    },
    {
        .name = "Test good requestline with path",
        .request = "GET /coffee HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n", 
        .wantMethod = "GET",
        .wantVersion = "HTTP/1.1",
        .wantTarget = "/coffee",
        .wantErr = PARSE_OK
    },
    {
        .name = "Invalid method",
        .request = "Get /coffee HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n",
        .wantMethod = NULL,
        .wantVersion = NULL,
        .wantTarget = NULL,
        .wantErr = ERR_INVALID_METHOD
    },
    {
        .name = "Invalid version",
        .request = "GET /coffee HTTP/2.0\r\nHost: localhost:8000\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n",
        .wantMethod = NULL,
        .wantVersion = NULL,
        .wantTarget = NULL,
        .wantErr = ERR_UNSUPPORTED_VERSION
    },
    {
        .name = "Invalid requestline",
        .request = "/coffeHTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n", 
        .wantMethod = NULL,
        .wantVersion = NULL,
        .wantTarget = NULL,
        .wantErr = ERR_BAD_REQUESTLINE
    },

};


static MunitResult test (const MunitParameter params[], void* fixture) {

    int size = sizeof(testCases) / sizeof(testCases[0]);

    for(int i = 0; i < size; i++){
        stringReader read = {
            .data = testCases[i].request,
            .totalLength = strlen(read.data),
            .pos = 0
        }; 
        reqStatus r = RequestFromReader(&read);
        if(testCases[i].wantMethod != NULL){
            munit_assert_string_equal(r.req->RequestLine.method, testCases[i].wantMethod);
            munit_assert_string_equal(r.req->RequestLine.requestTarget, testCases[i].wantTarget);
            munit_assert_string_equal(r.req->RequestLine.httpVersion, testCases[i].wantVersion);
            // munit_assert_int(r.status, ==, testCases[i].wantErr);
        } else {
            munit_assert_int(r.status, ==, testCases[i].wantErr);
        }
    }

    return MUNIT_OK;
}


//tests
MunitTest tests[] = {
    {
        "/my-test\n",
        test,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },

    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }

};


//test suite
static const MunitSuite suite = {
    "/my-tests",
    tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE

};



int main(int argc, char* argv[]){
    printf("Starting test suite\n");

    stringReader read = {
        .data = "GET / HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: curl/7.81.0\r\nContent-Length: 12\r\nAccept: */*\r\n\r\nhello server", 
        .totalLength = strlen(read.data),
        .pos = 0,
    };
    reqStatus r = RequestFromReader(&read);


    return 0;
    // return munit_suite_main(&suite, NULL, argc, argv);
}




// gcc -Irequest internal/headers/headers.c  internal/request/request.c internal/request/request_test.c -o nae -lmunit -lws2_32
