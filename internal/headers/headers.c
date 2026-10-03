#include "../../include/request.h"
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


Headers_t* headers_init(Headers_t *headers, int capacity) {
    headers->items = malloc(sizeof(Header) * capacity);
    headers->capacity = capacity;
    headers->size = 0;
}

void headers_put(Headers_t *headers, int index, char* key, char* value){
    printf("puting at index %d\n", index);
    if(index  > headers->capacity){
        printf("index is largeer than capacity\n");
        return;
    }

    Header head = {.key = strdup(key), .value = strdup(value)};
    headers->items[index] = head;
}

bool is_valid_header(char* field){
    char* specialCharacter = "!#$%&'*+-.^_`|~";
    for(int i = 0; i < strlen(field); i++){
        unsigned char c = (unsigned char)field[i];
        if(!isalnum(c) && strchr(specialCharacter, c) == NULL){
            printf("Field not valid %c\n", c);
            return false;
        }
    }
    printf("valid header field\n");
    return true;
}

int headers_getkey(Headers_t *headers, char* key){
    printf("getting key.....\n");
    if(!headers || !headers->items || !key) return -1;
    int status;
    for(int i =0; i < headers->size; i++){
        if(strcmp(headers->items[i].key, key) == 0){
            status = i;
            printf("%s %s from headers getting key at index %d\n", headers->items[i].key, key, i);
            return status;
        }
    }
    printf("Key not found\n");
    status = -1;
    return status;
}

void headers_push(Headers_t *headers, char* key, char* value){
    printf("pushing to headers...... \n");
    if(headers->size == headers->capacity){
        printf("Resizing headers\n");
        resize_headers(headers, headers->capacity *2);
    }

    headers->items[headers->size].key = strdup(key);
    headers->items[headers->size].value = strdup(value);
    headers->size++;

}
Header headers_get(Headers_t *headers, int index){

    printf("getting at index %d\n", index);
    char *key = headers->items[index].key;
    printf("%s\n", key);

    return headers->items[index];
}

void headers_delete(Headers_t *headers, int index){
    printf("deleting at index %d\n", index);
}




void resize_headers(Headers_t *header, int new_capacity){
    if(!header){ return;}

    Header * new_data = realloc(header->items, sizeof(Header) * new_capacity);
    if(!new_data) { return;}

    header->items =  new_data;
    header->capacity = new_capacity;
}


parseResult Parse(Request_t *req,int index, char *data ){
    printf("Parsing headers.........\n");

    char* newline = strstr(data, "\r\n");
    if(!newline){ return (parseResult) {0, ERR_INCOMPLETE}; }
    size_t totalParsed = newline - data + 2;

    if(data == newline || data[0] == '\r' && data[1] == '\n'){
        printf("header_ends here\n");
        return (parseResult) { totalParsed, HEADERS_DONE_PARSING};
    }

    char* duplicate = strdup(data);

    char* part1 = strtok(duplicate, " ");
    char* part2 = strtok(NULL, "\r\n");

    if(!part1 || !part2){
        free(duplicate);
        return (parseResult) {-1, ERR_BAD_HEADER_LINE};
    }
    if(strstr(part1, ":") == NULL){
        free(duplicate);
        return (parseResult) {-2, ERR_INVALID_FIELDNAME};
    }

    while(isspace((unsigned char)*part2)){
        part2++;
    }

    char *colon = strchr(part1, ':');
    *colon = '\0';

    bool ok = is_valid_header(part1);

    if(!ok){
        free(duplicate);
        return (parseResult) {-3, ERR_INVALID_CHAR_IN_FIELDNAME};
    }
    // tolower 
    for(int i = 0; i < strlen(part1); i++){
        part1[i] = tolower((unsigned char)part1[i]);
    }

    int get = headers_getkey(&req->Headers, part1);

    if(get >= 0){
        char* old_val = req->Headers.items[get].value;
        size_t new_len = strlen(old_val) + strlen(", ") + strlen(part2) + 1;

        char* new_val = realloc(old_val, new_len);
        if(!new_val){
            free(duplicate);
            return (parseResult) {-9,   ERR_NULL_POINTER};
        }
        req->Headers.items[get].value = new_val; 
        strcat(req->Headers.items[get].value, ", "); 
        strcat(req->Headers.items[get].value,part2); 
        free(duplicate);
        return (parseResult) {totalParsed, PARSE_OK };
    }


    headers_push(&req->Headers, part1, part2);
    free(duplicate);
    return (parseResult) { totalParsed, PARSE_OK };

}
