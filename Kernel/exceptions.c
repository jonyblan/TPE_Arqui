#include "videoDriver.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
} RegisterValues;

extern void fillRegisters(RegisterValues *RegisterValues);

void printRegisters(){
	RegisterValues regs;
	fillRegisters(&regs);
	puts("Registers values:");
	puts("rax: ");
	putChar(regs.rax);
    newLine();
	puts("rbx: ");
	putChar(regs.rbx);
	newLine();
	puts("rcx: ");
	putChar(regs.rcx);
	newLine();
	puts("rdx: ");
	putChar(regs.rdx);
	newLine();
	puts("r8: ");
	putChar(regs.r8);
	newLine();
	puts("r9: ");
	putChar(regs.r9);
	newLine();
	puts("r10: ");
	putChar(regs.r10);
	newLine();
	puts("r11: ");
	putChar(regs.r11);
	newLine();
	puts("r12: ");
	putChar(regs.r12);
	newLine();
	puts("r13: ");
	putChar(regs.r13);
	newLine();
	puts("r14: ");
	putChar(regs.r14);
	newLine();
}

void exceptionDispatcher(int exception) {
    switch(exception) {
        case ZERO_EXCEPTION_ID:
            putsf("Exception 0: Division by zero", BRIGHT_RED);
            break;
        case INVALID_OPCODE_ID:
            putsf("Exception 6: Invalid operation code", BRIGHT_RED);
            break;
        default:
            putsf("Uncontrolled exception", BRIGHT_RED);
    }
    printRegisters();
}