#ifndef USERLAND_LIBASM_H
#define USERLAND_LIBASM_H

#include <stdint.h>

//Funciones inspiradas en Linux

uint64_t sys_exit(int32_t ret);
/**
 * sysReadChar: inspirada en la funcion de Linux, lee de a un char en lugar de un str y lo devuelve.
 */
char sysReadChar(uint32_t fileDesc);
/**
 * sysWriteColor: inspirada en la funcion de Linux, imprime un string con color en formato 0x00RRGGBB.
 */
uint64_t sysWriteColor(uint32_t fileDesc, char* source, uint64_t len, uint32_t hexColor);
uint64_t sys_execve(const char * progPath, uint64_t argc, char * const argv[]);;
uint64_t sys_time(int32_t * dest);

//Funciones definidas
void callNewLine();
void callPutPixel(uint32_t hexColor, uint64_t x, uint64_t y);

#endif