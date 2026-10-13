#pragma once
#include <stdbool.h>

typedef struct Header {
    char* key;
    char* value;
}Header;

typedef struct Headers {
    Header* items;
    int capacity;
    int size;
}Headers_t;

bool is_valid_header(char* field);
void headers_init(Headers_t *headers, int capacity);
void resize_headers(Headers_t *header, int capacity);

int headers_getkey(Headers_t* headers, char* key);
void headers_push(Headers_t *headers, char*key, char* value);
void headers_put(Headers_t *headers, char* key, char* value);
void replace_header(Headers_t *headers, char* oldkey ,char* key, char* value);
void delete_header(Headers_t *headers, char* key);


