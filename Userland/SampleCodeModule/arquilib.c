#include <stdint.h>
#include "arquilib.h"
#include "libasm.h"
#include "utillib.h"

void printc(const char * string, uint32_t hexColor){
    sysWriteColor(STANDARD_OUTPUT, string, strlen(string), hexColor);
}

void print(const char * string){
    printc(string, WHITE);
}

void putCharc(const char c, uint32_t hexColor){
    printc(&c, hexColor);
}

void putChar(const char c){
    putCharc(c, WHITE);
}

void puts(const char * string){
    print(string);
    callNewLine();
}

char getChar(){
    return sysReadChar(STANDARD_INPUT);
}

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y){
    callPutPixel(hexColor, x, y);
}

void clear(){
    callClear();
}

void scale(char * argv){
    if(strlen(argv) != 1){
        printc("Illegal argument\n", RED);
    } else {
        callScale(argv[0]-'0');
    }
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