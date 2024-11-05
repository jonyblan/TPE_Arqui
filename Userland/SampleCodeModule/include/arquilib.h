#ifndef USERLAND_ARQUILIB_H
#define USERLAND_ARQUILIB_H

#define STANDARD_INPUT 0
#define STANDARD_OUTPUT 1

void putChar(const char c);
void puts(const char * c);
char getChar();
void printDateTime();


int getTicks();

#endif