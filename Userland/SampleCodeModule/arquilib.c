#include <stdint.h>
#include "arquilib.h"
#include "libasm.h"
#include "utillib.h"

void printc(const char * string, uint32_t hexColor){
    sysWriteColor(STANDARD_OUTPUT, string, strlen(string), hexColor);
}

void printcint(int num, uint32_t hexColor){
	// chatgpt's way of turning an int into a char*
	static char buffer[12]; // Enough space for the largest 32-bit integer, including the null terminator
	int index = 0;

	// Handle special case for 0
	if (num == 0) {
		buffer[index++] = '0';
	} 
	else {
		// Handle negative numbers
		if (num < 0) {
			buffer[index++] = '-';
			num = -num; // Work with positive equivalent
		}

		// Convert number to string (reverse order)
		int start = index; // Mark where digits begin
		while (num > 0) {
			buffer[index++] = '0' + (num % 10);
			num /= 10;
		}

		// Reverse the digits (if applicable)
		for (int i = start, j = index - 1; i < j; ++i, --j) {
			char temp = buffer[i];
			buffer[i] = buffer[j];
			buffer[j] = temp;
		}
	}

	// Null terminate the string
	buffer[index] = '\0';

	printc(buffer, hexColor);
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