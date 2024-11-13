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
	print("rax: ");
	printc(itoa(regs.rax), TURQUOISE);
	print("\trbx: ");
	printc(itoa(regs.rbx), TURQUOISE);
	print("\trcx: ");
	printc(itoa(regs.rcx), TURQUOISE);
	print("\trdx: ");
	printc(itoa(regs.rdx), TURQUOISE);
	print("\tr8: ");
	printc(itoa(regs.r8), TURQUOISE);
	print("\tr9: ");
	printc(itoa(regs.r9), TURQUOISE);
	print("\nr10: ");
	printc(itoa(regs.r10), TURQUOISE);
	print("\tr11: ");
	printc(itoa(regs.r11), TURQUOISE);
	print("\tr12: ");
	printc(itoa(regs.r12), TURQUOISE);
	print("\tr13: ");
	printc(itoa(regs.r13), TURQUOISE);
	print("\tr14: ");
	printc(itoa(regs.r14), TURQUOISE);
}

void exceptionDispatcher(int exception) {
    switch(exception) {
        case ZERO_EXCEPTION_ID:
            putsc("\nException 0: Division by zero", RED);
            break;
        case INVALID_OPCODE_ID:
            putsc("\nException 6: Invalid operation code", RED);
            break;
        default:
            putsc("\nUncontrolled exception", RED);
    }
    printRegisters();
    //timer
    resetMain();
}