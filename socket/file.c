#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


void getLinesChannel(FILE *file){
    int n;
    char str[128] = "";
    char buf[1024] ={0};

    while(1){
        n = fread(buf, 1, 8, file);
        if(n<= 0){ break; }

        buf[n] = '\0';
        char *newline = strstr(buf,"\n");

        if(newline){ 
            strncat(str, buf, newline - buf);
            printf("read: %s\n", str);
            str[0] = '\0';
            strcat(str, newline + 1);
        }else {
            strcat(str, buf);
        }
    }
    return;
}

int main () {
    FILE *f = fopen("./socket/file.txt", "rb");
    if( f == NULL ){
        perror("error opening file");
        return -1;
    }

    getLinesChannel(f);

    if(feof(f)){
        printf("end of file reached\n");
    }
    fclose(f);
    return 0;
}



