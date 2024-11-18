#include "utillib.h"

uint64_t strlen(const char * string){
    uint64_t ret = 0;
    while(string[ret]!=0){
        ret++;
    }
    return ret;
}

int8_t strcmp(const char* str1, const char* str2){
    int8_t cmp = 0;
    for(int i = 0; str1[i]!=0 && str2[i]!=0 && cmp == 0; i++){
        if(str1[i+1] == 0 && str2[i+1] != 0) {cmp = -1;}
        else if(str1[i+1] != 0 && str2[i+1] == 0) {cmp = 1;}
        else { cmp = str1[i]-str2[i]; }
    }
    return cmp;
}

void strcpyUntil(char * dest, char * src, char stop){
    if(src == 0 || src[0] == 0){
        dest[0] = 0;
    }

    int i = 0;
    for(; src[i]!=0 && src[i]!=stop; i++){
        dest[i] = src[i];
    }
    dest[i] = 0;
}

void strcpyFrom(char * dest, char * src, char start){
    if(src == 0 || src[0] == 0){
        dest[0] = 0;
    }

    int i = 0;
    while(src[i]!=start){
        i++;
    }
    i++;
    int j = 0;
    for(; src[i]!=0; i++, j++){
        dest[j] = src[i];
    }
    dest[j] = 0;
}