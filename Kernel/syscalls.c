#include <stdint.h>
#include "videoDriver.h"
#include "keyboard.h"
#include "time.h"
#include "interrupts.h"
#include "syscalls.h"
#include "exceptions.h"

#define STANDARD_INPUT 0
#define STANDARD_OUTPUT 1

#define SYSCALL_COUNT 195
//todo: cambiar el valor a medida que agregue syscalls

static uint64_t sys_exit_impl(int32_t ret);
char sysReadCharImpl(uint32_t fileDesc);
uint64_t sysWriteColorImpl(uint32_t fileDesc, const char* source, uint64_t len, uint32_t hexColor);
//static uint64_t sys_execve_impl(const char * progPath, uint64_t argc, char * const argv[]);
static uint64_t sysTimeImpl(int32_t * dest);

void callNewLine();
void callPutPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void callClear();
void callPrintRegs();

typedef uint64_t (*Syscall)(uint64_t, uint64_t, uint64_t, uint64_t);
static Syscall syscalls[SYSCALL_COUNT];

void fillSyscalls(){
    //syscalls[1] = (Syscall) sys_exit_impl;
    syscalls[3] = (Syscall) sysReadCharImpl;
    syscalls[4] = (Syscall) sysWriteColorImpl;
    //syscalls[11] = (Syscall) sys_execve_impl;
    syscalls[13] = (Syscall) sysTimeImpl;
    syscalls[191] = (Syscall) callNewLine;
    syscalls[192] = (Syscall) callPutPixel;
    syscalls[193] = (Syscall) callClear;
    syscalls[194] = (Syscall) callPrintRegs;
}

//los registros como parametros de C son del orden:
//RDI RSI RDX RCX R8 R9 (usamos 5)
//Estan decalados en 1 en interrupts.asm, por lo que corresponden a:
//RAX RDI RSI RDX RCX R8 (usamos 5)

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8){
    if(rdi<SYSCALL_COUNT && syscalls[rdi]!=0){
        return syscalls[rdi](rsi, rdx, r10, r8);
    }
    return 1;
}

/*
static uint64_t sys_exit_impl(int32_t ret){
    return sys_exit(ret);
}
 */

char sysReadCharImpl(uint32_t fileDesc){
    if(fileDesc != STANDARD_INPUT) { return 0; }
    /*
    for(uint64_t i=0; i<len; i++){
        char c = getMsg();
        if(c==0) { return i; }
        dest[i] = c;
    }
    */
    return getMsg();
}

uint64_t sysWriteColorImpl(uint32_t fileDesc, const char* source, uint64_t len, uint32_t hexColor){
    if(fileDesc != STANDARD_OUTPUT) {return -1;}
    printc(source, hexColor);
    return len;
}

/*
static uint64_t sys_execve_impl(const char * progPath, uint64_t argc, char * const argv[]){
    return sys_execve(progPath, argc, argv);
}
 */

static uint64_t sysTimeImpl(int32_t * dest){
    //todo
}

void callNewLine(){
    newLine();
}

void callPutPixel(uint32_t hexColor, uint64_t x, uint64_t y){
    putPixel(hexColor, x, y);
}

void callClear(){
    clear();
}

void callPrintRegs(){
    //solucion simple pero algo fea
    _printRegisters();
}