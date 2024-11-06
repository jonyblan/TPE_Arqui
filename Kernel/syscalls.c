#include <stdint.h>
#include "videoDriver.h"
#include "keyboard.h"
#include "time.h"
#include "interrupts.h"
#include "syscalls.h"

#define STANDARD_INPUT 0
#define STANDARD_OUTPUT 1

#define SYSCALL_COUNT 190
//todo: cambiar el valor a medida que agregue syscalls

static uint64_t sys_exit_impl(int32_t ret);
static uint64_t sys_read_impl(uint32_t fileDesc, char* dest, uint64_t len);
static uint64_t sys_write_impl(uint32_t fileDesc, const char* source, uint64_t len);
static uint64_t sys_execve_impl(const char * progPath, uint64_t argc, char * const argv[]);
static uint64_t sys_time_impl(int32_t * dest);

typedef uint64_t (*Syscall)(uint64_t, uint64_t, uint64_t);
static Syscall syscalls[SYSCALL_COUNT];

void fillSyscalls(){
    //syscalls[1] = (Syscall) sys_exit_impl;
    syscalls[3] = (Syscall) sys_read_impl;
    syscalls[4] = (Syscall) sys_write_impl;
    //syscalls[11] = (Syscall) sys_execve_impl;
    syscalls[13] = (Syscall) sys_time_impl;
}

uint64_t syscallDispatcher(uint64_t rax, uint64_t arg1, uint64_t arg2, uint64_t arg3){
    if(rax<SYSCALL_COUNT && syscalls[rax]!=0){
        return syscalls[rax](arg1, arg2, arg3);
    }
    return 1;
}

/*
static uint64_t sys_exit_impl(int32_t ret){
    return sys_exit(ret);
}
 */

static uint64_t sys_read_impl(uint32_t fileDesc, char* dest, uint64_t len){
    if(fileDesc != STANDARD_INPUT) { return 0; }
    for(uint64_t i=0; i<len; i++){
        char c = getMsg();
        if(c==0) { return i; }
        dest[i] = c;
    }
    return len;
}

static uint64_t sys_write_impl(uint32_t fileDesc, const char* source, uint64_t len){
    if(fileDesc != STANDARD_OUTPUT) {return -1;}
    //source[len] = 0; //por las dudas lo hago null term
    puts(source);
    return len;
}

/*
static uint64_t sys_execve_impl(const char * progPath, uint64_t argc, char * const argv[]){
    return sys_execve(progPath, argc, argv);
}
 */

static uint64_t sys_time_impl(int32_t * dest){
    //todo
}