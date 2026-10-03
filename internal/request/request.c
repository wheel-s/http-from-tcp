#include "../../include/request.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")






parseResult parseRequestLine(char *reqLine, Request_t *req){
    if(!req || !reqLine){ return (parseResult) { -6, ERR_NULL_POINTER }; }

    char *newline = strstr(reqLine, "\r\n");
    if(!newline){
        return (parseResult) { 0, ERR_INCOMPLETE };
    }
    char* duplicate = strdup(reqLine);

    char *method = strtok(duplicate, " ");
    char *path = strtok(NULL, " ");
    char *version = strtok(NULL, "\r\n");

    if(!method || !path || !version){
        return (parseResult) {-1, ERR_BAD_REQUESTLINE };
    }

    if(strpbrk(method, "0123456789") != NULL){
        return (parseResult) { -2, ERR_INVALID_METHOD };
    }

    for(int i=0; method[i]; i++){
         if(!isupper(method[i])){ 
             return (parseResult) { -3, ERR_INVALID_METHOD };
         }
    }

    if(path[0] != '/'){
        return (parseResult) { -4, ERR_INVALID_PATH };
    }
    if(strcmp(version, "HTTP/1.1") != 0){
        return (parseResult) { -5, ERR_UNSUPPORTED_VERSION };
    }

    req->RequestLine.method = strdup(method);
    req->RequestLine.requestTarget = strdup(path);
    req->RequestLine.httpVersion = strdup(version);

    free(duplicate);
    int totalRead = strlen(reqLine);
    size_t totalParsed = newline - reqLine + 2;
    return (parseResult) { totalParsed, PARSE_OK };

}


parseResult parse(Request_t *req, char *data, int len){

    if(req->state ==INITIALIZED){

        parseResult result = parseRequestLine(data, req);
        if(result.read < 0){
            req->state = DONE;
            return result;
        } 
        if( result.read > 0){
            req->state = requestStateParsingHeaders;
            return result;
        }
        if( result.read == 0){
            return result;
        }
    }
    if(req->state == requestStateParsingHeaders){

        parseResult re =  Parse(req, len, data);
        if( re.status == HEADERS_DONE_PARSING){
            req->state = parsingbody;
            return re;
        }
       if(re.read < 0){
            req->state = DONE;
            return re;
        } 

        return re;
    }
    if(req->state == parsingbody){
        int get = headers_getkey(&req->Headers, "content-length");
        if(get < 0){
           req->state = DONE;
           return (parseResult) { 0, PARSE_OK };
        }
        size_t content_length = (size_t)atoi(req->Headers.items[get].value);
        printf("%zu\n", content_length);

        req->body = malloc(content_length +1);

        if(strlen(data) < content_length ){
            req->body = data;
            return (parseResult){ 0, ERR_INCOMPLETE};
        }

        if(strlen(data) > content_length){
            req->state = DONE;
            return (parseResult){ -6, ERR_BAD_BODY };
        }
        req->body = data;
        req->state = DONE;
        return (parseResult){ strlen(data), PARSE_OK};

    }
    if(req->state == DONE){
        return (parseResult){ -7, TRYING_TO_READ_DATA_IN_A_DONE_STATE };
    }

    if(!req->state){
        return (parseResult) {-2, UNKNOWN_STATE };
    }
}


int readChunk(stringReader *read, char *dest, int toRead){
    int remaining = read->totalLength - read->pos;

    if(remaining <= 0){
        printf("end of file reached\n");
        return 0;
    }
    if(toRead > remaining){
        toRead = remaining;
    }
    memcpy(dest, read->data +read->pos, toRead);
    read->pos = read->pos + toRead;
    return toRead;
}

const char* parse_error_str(parseStatus err) {
    switch(err) {
        case PARSE_OK: return "Success";
        case ERR_INCOMPLETE: return "Incomplete request, need more data";
        case ERR_LINE_TOO_LONG: return "Request line exceed limits";
        case ERR_INVALID_METHOD: return "Invalid HTTP method";
        case ERR_INVALID_PATH: return "Invalid path (must start with/)";
        case ERR_BAD_REQUESTLINE: return "Malformed HTTP request line";
        case ERR_UNSUPPORTED_VERSION: return "Unsupported HTTP version";
        case ERR_BAD_HEADER_LINE: return " Malformed header line";
        case HEADERS_DONE_PARSING: return "HEADERS DONE PARSING";
        case ERR_INVALID_FIELDNAME: return "Invalid header field name missing :";
        case ERR_INVALID_CHAR_IN_FIELDNAME: return "Invalid character header field name";
        case TRYING_TO_READ_DATA_IN_A_DONE_STATE: return "Trying to read data on done state";
        case ERR_BAD_BODY: return "Error bad body large content length";
        case ERR_NULL_POINTER: return "Error Null pointer";
        case UNKNOWN_STATE: return "Unknown state";
        defult: return "Unknown error";
    }
}

reqStatus RequestFromSocket(SOCKET client){

    int bytes_read;
    int readToIndex = 0;
    size_t capacity =8;
    size_t remaining;
    bool need = true;
    char *dest = malloc(capacity +1);
    parseResult result;

    Request_t *req = malloc(sizeof(Request_t));
    headers_init(&req->Headers, 4);
    req->state = INITIALIZED;

    while ( req->state != DONE){

        dest[readToIndex] = '\0';
        result = parse(req, dest, readToIndex);

    if(result.status == ERR_INCOMPLETE){
        if( readToIndex +8 > capacity ){
            printf("Reallocating destination buffer\n");
            capacity *=2;
            char* new_dest = realloc(dest, capacity);
            if(!new_dest) {
                free(dest);
                return (reqStatus) { ERR_NULL_POINTER, req };
            }
            dest = new_dest;
        }

        bytes_read = recv(client, dest + readToIndex, 8, 0);
        if(bytes_read > 0){
            readToIndex += bytes_read;
            dest[readToIndex] = '\0';
        }
        else if (bytes_read == 0) {
            req->state = DONE;
            break;
        }
        else {
            //socket error
            break;
        }
    }

        const char* stat = parse_error_str(result.status);
        printf("%d  %s\n",result.read, stat);
        printf("%.*s\n",(int)readToIndex, dest);
        if(result.status == PARSE_OK || result.status == HEADERS_DONE_PARSING){
            remaining = readToIndex - result.read;
            memmove(dest, dest + result.read, remaining);
            readToIndex = remaining;
            dest[readToIndex] = '\0';
        }

    }

    dest[readToIndex] = '\0';
    printf("%s\n", dest);

    reqStatus val = {
        .status = result.status,
        .req = req
    };
    return val ;
}

reqStatus RequestFromReader(stringReader *read){

    int bytes_read;
    int readToIndex = 0;
    size_t capacity = 4;
    size_t remaining;

    char *dest = malloc(capacity + 1);
    parseResult result;

    Request_t *req = malloc(sizeof(Request_t));
    headers_init(&req->Headers, 1);
    req->state = INITIALIZED;

    while ( req->state != DONE){

        if( readToIndex + 4  > capacity ){
            printf("Reallocating destination buffer\n");
            capacity *=2;
            char* new_dest = realloc(dest, capacity + 1);
            if(!new_dest) {
                free(dest);
                return (reqStatus) { ERR_NULL_POINTER, req };
            }
            dest = new_dest;
        }
        bytes_read = readChunk(read, dest + readToIndex, 4);

        readToIndex += bytes_read;
        if(bytes_read == 0) {
            // req->state = DONE;
        }

        dest[readToIndex] = '\0';
        result = parse(req, dest, readToIndex);

        const char* stat = parse_error_str(result.status);
        printf("%d  %s\n",result.read, stat);
        printf("%s\n", dest);

        if(result.status == PARSE_OK || result.status == HEADERS_DONE_PARSING){
            remaining = readToIndex - result.read;
            memmove(dest, dest + result.read, remaining);
            readToIndex = remaining;
            dest[readToIndex] = '\0';
        }

    }

    printf("%s\n", dest);

    reqStatus val = {
        .status = result.status,
        .req = req
    };
    return val ;
}


