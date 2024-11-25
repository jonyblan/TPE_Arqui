#ifndef USERLAND_LIBASM_H
#define USERLAND_LIBASM_H

#include <stdint.h>

//Funciones inspiradas en Linux

/**
 * sysReadChar: inspirada en la funcion de Linux, lee de a un char en lugar de un str y lo devuelve.
 */
char sysReadChar(uint32_t fileDesc);
/**
 * sysWriteColor: inspirada en la funcion de Linux, imprime un string con color en formato 0x00RRGGBB.
 */
uint64_t sysWriteColor(uint32_t fileDesc, const char* source, uint64_t len, uint32_t hexColor);
uint64_t sys_time(int32_t * dest);

//Funciones definidas
void callNewLine();
void callPutPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void callClear();
void callScale(uint8_t factor);
void callSetCursor(uint8_t x, uint8_t y);
void callPrintRegs();
void callPrintSystemTime();
uint64_t callTicksElapsed();

//Excepciones
extern void excInvalidOpCode();
extern void excDiv0();

#endif