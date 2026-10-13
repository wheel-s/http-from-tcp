#include "request.h"
#include <munit.h>


typedef struct testCase {
    char*   name;
    char *request;
    char*   wantKey;
    char*   wantValue;
    int  wantRead;
    parseStatus wantErr;
} testCase;


testCase testCases[] = {
    {
        .name = "Standard headers",
        .request = "GET / HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n", 
        .wantKey = "host",
        .wantValue = "localhost:8000",
        .wantRead = 20,
        .wantErr = PARSE_OK
    },
    {
        .name = "Duplicate headers",
        .request = "GET / HTTP/1.1\r\nHost: localhost:8000\r\nHost: localhost:9000\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n", 
        .wantKey = "user-agent",
        .wantValue = "curl/7.81.0",
        .wantRead = 20,
        .wantErr = PARSE_OK
    },
     {
        .name = "Invalid spacing",
        .request = "GET / HTTP/1.1\r\nHost: localhost:8000\r\nHost: localhost:9000\r\nUser-Agent : curl/7.81.0\r\nAccept: */*\r\n\r\n", 
        .wantKey = NULL,
        .wantValue = NULL,
        .wantRead = 1,
        .wantErr = ERR_INVALID_FIELDNAME
    },
    {
        .name = "Invalid character",
        .request = "GET / HTTP/1.1\r\nHos@t: localhost:8000\r\nHost: localhost:9000\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n", 
        .wantKey = NULL,
        .wantValue = NULL,
        .wantRead = 0,
        .wantErr = ERR_INVALID_CHAR_IN_FIELDNAME 
    },
    {
        .name = "Malformed headers",
        .request = "GET / HTTP/1.1\r\nlocalhost:8000\r\nHost: localhost:9000\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\n", 
        .wantKey = NULL,
        .wantValue = NULL,
        .wantRead = 0,
        .wantErr = ERR_BAD_HEADER_LINE
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
        if(testCases[i].wantKey != NULL){
            munit_assert_string_equal(r.req->Headers.items[i].key, testCases[i].wantKey);
            munit_assert_string_equal(r.req->Headers.items[i].value, testCases[i].wantValue);
            munit_assert_int(r.status, ==, testCases[i].wantErr);

        } else {
            if(r.req !=NULL){
                munit_assert_size(r.req->Headers.size, ==, testCases[i].wantRead);
                munit_assert_int(r.status, ==, testCases[i].wantErr);
            }
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
    printf("test parsing headers........\n");

    return munit_suite_main(&suite, NULL, argc, argv);
}

