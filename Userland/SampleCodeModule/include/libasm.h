#ifndef USERLAND_LIBASM_H
#define USERLAND_LIBASM_H

#include <stdint.h>

uint64_t sys_exit(int32_t ret);
uint64_t sys_read(uint32_t fileDesc, char* dest, uint64_t len);
uint64_t sys_write(uint32_t fileDesc, const char* source, uint64_t len);
uint64_t sys_execve(const char * progPath, uint64_t argc, char * const argv[]);;
uint64_t sys_time(int32_t * dest);

//Funciones definidas

#endif