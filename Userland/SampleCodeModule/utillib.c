#include "utillib.h"

uint64_t strlen(const char * string){
    uint64_t ret = 0;
    while(string[ret]!='\0'){
        ret++;
    }
    return ret;
}

int8_t strcmp(const char* str1, const char* str2){
    int8_t cmp = 0;
    for(int i = 0; str1[i]!='\0' && str2[i]!='\0' && cmp == 0; i++){
        if(str1[i+1] == '\0' && str2[i+1] != '\0') {cmp = -1;}
        if(str1[i+1] != '\0' && str2[i+1] == '\0') {cmp = 1;}
        else { cmp = str1[i]-str2[i]; }
    }
    return cmp;
}