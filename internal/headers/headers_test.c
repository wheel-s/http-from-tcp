#include "../../include/request.h"
#include "assert.h"
#include "stdio.h"
#include "stdlib.h"


int main(){
        
     
    char *data = "User-Agent: curl/8.16.0\r\n\r\n";
    
    Request_t *req = malloc(sizeof(req));
    for(int i=0; i <4; i++){
    
       int p =  Parse(req, i,  data);
       
    }
    printf("%s\n%s\n", req->Headers[0].key, req->Headers[0].value);
    free(req);
    return 0;

}