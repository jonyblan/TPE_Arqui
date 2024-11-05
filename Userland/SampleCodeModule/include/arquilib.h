#ifndef USERLAND_ARQUILIB_H
#define USERLAND_ARQUILIB_H

#include <stdint.h>

#define STANDARD_INPUT 0
#define STANDARD_OUTPUT 1

void putChar(const char c);
void puts(const char * string);
char getChar();
int8_t strcmp(const char* str1, const char* str2);
void printDateTime();


int getTicks();

#endif