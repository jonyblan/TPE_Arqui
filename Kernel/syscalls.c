#include <stdint.h>
#include "videoDriver.h"
#include "keyboard.h"
#include "time.h"
#include "interrupts.h"
#include "syscalls.h"
#include "exceptions.h"
#include "beepDriver.h";

#define STANDARD_INPUT 0
#define STANDARD_OUTPUT 1

#define SYSCALL_COUNT 198
//todo: cambiar el valor a medida que agregue syscalls

void sysExitImpl();
char sysReadCharImpl(uint32_t fileDesc);
uint64_t sysWriteColorImpl(uint32_t fileDesc, const char* source, uint64_t len, uint32_t hexColor);
uint64_t sysTimeImpl(int32_t * dest);

void callBeep();
void callNewLine();
void callPutPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void callClear();
void callScale(uint8_t factor);
void callPrintRegs();
void callPrintSystemTime();
uint64_t callTicksElapsed();

typedef uint64_t (*Syscall)(uint64_t, uint64_t, uint64_t, uint64_t);
static Syscall syscalls[SYSCALL_COUNT];

void fillSyscalls(){
    //syscalls[1] = (Syscall) sysExitImpl;
    syscalls[3] = (Syscall) sysReadCharImpl;
    syscalls[4] = (Syscall) sysWriteColorImpl;
    syscalls[6] = (Syscall) callBeep;
    syscalls[13] = (Syscall) sysTimeImpl;
    syscalls[191] = (Syscall) callNewLine;
    syscalls[192] = (Syscall) callPutPixel;
    syscalls[193] = (Syscall) callClear;
    syscalls[194] = (Syscall) callScale;
    syscalls[195] = (Syscall) callPrintRegs;
    syscalls[196] = (Syscall) callPrintSystemTime;
    syscalls[197] = (Syscall) callTicksElapsed;
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


void sysExitImpl(){
    while(1){
        _hlt;
    }
}

char sysReadCharImpl(uint32_t fileDesc){
    if(fileDesc != STANDARD_INPUT) { return 0; }
    return getMsg();
}

uint64_t sysWriteColorImpl(uint32_t fileDesc, const char* source, uint64_t len, uint32_t hexColor){
    if(fileDesc != STANDARD_OUTPUT) {return -1;}
    printc(source, hexColor);
    return len;
}

uint64_t sysTimeImpl(int32_t * dest){
    //note: la syscall de linux devuelve el tiempo desde epoch (1/1/1970).
    //Esta implementacion devuelve el tiempo desde que se inicio el sistema
    //Otra syscall imprime la fecha y hora actual
    return seconds_elapsed();
}

void callBeep(){
	beep_driver();
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

void callScale(uint8_t factor){
    if(factor >= 1 && factor <= 4){
        clear();
        setScale(factor);
    } else {
        printc("Factor out of range\n", RED);
    }
}

void callPrintRegs(){
    //solucion simple pero algo fea
    _printRegisters();
}

void callPrintSystemTime(){
    uint64_t time = get_sys_time();
    char * str = 0;
    itoa(time, str);
    //formato recibido: YYMMDDhhmmss
    //formato a imprimir: hh:mm:ss - DD/MM/20YY
    putCharc(str[6], MINT);
    putCharc(str[7], MINT);
    putCharc(':', MINT);
    putCharc(str[8], MINT);
    putCharc(str[9], MINT);
    putCharc(':', MINT);
    putCharc(str[10], MINT);
    putCharc(str[11], MINT);
    printc(" - ", MINT);
    putCharc(str[4], MINT);
    putCharc(str[5], MINT);
    putCharc('/', MINT);
    putCharc(str[2], MINT);
    putCharc(str[3], MINT);
    putCharc('/', MINT);
    printc("20", MINT);
    putCharc(str[0], MINT);
    putCharc(str[1], MINT);
    putChar('\n');
}

uint64_t callTicksElapsed(){
    return ticks_elapsed();
}