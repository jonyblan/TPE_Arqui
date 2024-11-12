#ifndef USERLAND_ARQUILIB_H
#define USERLAND_ARQUILIB_H

#include <stdint.h>

#define STANDARD_INPUT 0
#define STANDARD_OUTPUT 1

enum Colors { WHITE=0x00FFFFFF, BLACK=0x00000000, RED=0x00FF0000, GREEN=0x0000FF00, BLUE=0x000000FF,
    ORANGE=0x00FF8000, YELLOW=0x00FFFF00, LIME=0x0080FF00, MINT=0x0000FF80, TURQUOISE=0x0000FFFF,
    AQUA=0x000080FF, PURPLE=0x008000FF, MAGENTA=0x00FF00FF, PINK=0x00FF0080,
    LINUX_GREEN=0x0016C60C, LINUX_BLUE=0x002668FF};

void printc(const char * string, uint32_t hexColor);
void print(const char * string);
void putChar(const char c);
void puts(const char * string);
char getChar();

void clear();
/*
void upscale();
void downscale();
void setCursor();
*/
#endif