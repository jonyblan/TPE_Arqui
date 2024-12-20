#ifndef LIB_H
#define LIB_H

#include <stdint.h>

extern char *cpuVendor(char *result);
extern char getInput();

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

#endif