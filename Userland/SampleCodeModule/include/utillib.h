#ifndef USERLAND_UTILLIB_H
#define USERLAND_UTILLIB_H

#include <stdint.h>

uint64_t strlen(const char * string);
int8_t strcmp(const char* str1, const char* str2);
void strcpyUntil(char * dest, char * src, char stop);
void strcpyFrom(char * dest, char * src, char start);

#endif
