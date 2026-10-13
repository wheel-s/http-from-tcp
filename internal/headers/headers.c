#include "request.h"


void headers_init(Headers_t *headers, int capacity) {
    headers->items = malloc(sizeof(Header) * capacity);
    headers->capacity = capacity;
    headers->size = 0;
}

void resize_headers(Headers_t *header, int new_capacity){
    if(!header){ return;}

    Header * new_data = realloc(header->items, sizeof(Header) * new_capacity);
    if(!new_data) { return;}

    header->items =  new_data;
    header->capacity = new_capacity;
}

bool is_valid_header(char* field){
    char* specialCharacter = "!#$%&'*+-.^_`|~";
    for(int i = 0; i < strlen(field); i++){
        unsigned char c = (unsigned char)field[i];
        if(!isalnum(c) && strchr(specialCharacter, c) == NULL){
            // printf("Field not valid %c\n", c);
            return false;
        }
    }
    // printf("valid header field\n");
    return true;
}

int headers_getkey(Headers_t *headers, char* key){
    // printf("getting key.....\n");
    if(!headers || !headers->items || !key) return -1;
    int status;
    for(int i =0; i < headers->size; i++){
        if(strcasecmp(headers->items[i].key, key) == 0){
            status = i;
            // printf("%s %s from headers getting key at index %d\n", headers->items[i].key, key, i);
            return status;
        }
    }
    // printf("Key not found\n");
    status = -1;
    return status;
}

void headers_put(Headers_t *headers, char* key, char* value){
    int index = headers_getkey(headers, key);

    if(index < 0){
        // printf("can't update key that doesn't exist");
        return;
    }

    // printf("puting at index %d\n", index);
    headers->items[index].key = strdup(key);
    headers->items[index].value = strdup(value);
    return;
}

void replace_header(Headers_t* headers, char* oldkey, char* key, char* value) {
    int index = headers_getkey(headers, oldkey);

    if(index < 0){
        // printf("can't update key that doesn't exist");
        return;
    }

    headers->items[index].key = strdup(key);
    headers->items[index].value = strdup(value);
    return;
}

void headers_push(Headers_t *headers, char* key, char* value){
    // printf("pushing to headers...... \n");
    if(headers->size == headers->capacity){
        // printf("Resizing headers\n");
        resize_headers(headers, headers->capacity *2);
    }

    headers->items[headers->size].key = strdup(key);
    headers->items[headers->size].value = strdup(value);
    headers->size++;

}

void delete_header(Headers_t *headers, char* key){
    int index = headers_getkey(headers, key);

    if(index < 0){
        // printf("can't delete key that doesn't exist");
        return;
    }

    headers->items[index].key = NULL;
    headers->items[index].value = NULL;

    return;
}




parseResult Parse(Request_t *req, char* data, int len){
    // printf("Parsing headers.........\n");

    char* newline = strstr(data, "\r\n");
    if(!newline){ return (parseResult) {0, ERR_INCOMPLETE}; }
    size_t totalParsed = newline - data + 2;

    if(data == newline || (data[0] == '\r' && data[1] == '\n')){
        // printf("header_ends here\n");
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
