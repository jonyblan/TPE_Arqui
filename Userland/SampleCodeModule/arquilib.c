#include <stdint.h>
#include "arquilib.h"
#include "libasm.h"
#include "utillib.h"

void printc(const char * string, uint32_t hexColor){
    sysWriteColor(STANDARD_OUTPUT, string, strlen(string), hexColor);
}

void print(const char * string){
    //sysWriteColor(STANDARD_OUTPUT, string, strlen(string), 0x00FFFFFF);
    printc(string, WHITE);
}

void putCharc(const char c, uint32_t hexColor){
    printc(&c, hexColor);
}

void putChar(const char c){
    //sysWriteColor(STANDARD_OUTPUT, &c, 1, 0x00FFFFFF);
    putCharc(c, WHITE);
}

void puts(const char * string){
    print(string);
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

//TODO: scan (leer mas de un char)

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y){
    callPutPixel(hexColor, x, y);
}



void clear(){
    callClear();
}

/*
void upscale(){
    callUpscale();
}

void downscale(){
    callDownscale();
}

void setCursor(uint8_t x, uint8_t y){
  callSetCursor(x, y);
}
 */