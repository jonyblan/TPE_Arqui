#include <stdint.h>
#include "arquilib.h"
#include "libasm.h"

void putChar(const char c){
    sysWrite(STANDARD_OUTPUT, &c, 1);
}

//TODO: imprimir caracter con color
/*
void putCharf(const char c, uint32_t hexColor){

}
 */

uint64_t strlen(const char * string){
    uint64_t ret = 0;
    while(string[ret]!='\0'){
        ret++;
    }
    return ret;
}

void puts(const char * string){
    sysWrite(STANDARD_OUTPUT, string, strlen(string));
    callNewLine();
}

char getChar(){
    /*
    //puts("getChar");
    char c = 0;
    c = sysReadChar(STANDARD_INPUT);
    if(c>=0){
        //puts("c>=0");
        return c;
    }
    return c;
     */
    return sysReadChar(STANDARD_INPUT);
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

//TODO: scan (leer mas de un char)

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y){
    callPutPixel(hexColor, x, y);
}