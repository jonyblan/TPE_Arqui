#include "videoDriver.h"
#include "time.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

void printRegisters(uint64_t regs[18]){
	char buffer[10];
	puts("Registers values:");
	print("rip: ");
    itoa(regs[0], buffer);
	printc(buffer, TURQUOISE);
	print("\trax: ");
    itoa(regs[0], buffer);
	printc(buffer, TURQUOISE);
	print("\trbx: ");
    itoa(regs[1], buffer);
	printc(buffer, TURQUOISE);
	print("\trcx: ");
	itoa(regs[2], buffer);
	printc(buffer, TURQUOISE);
	print("\trdx: ");
	itoa(regs[3], buffer);
	printc(buffer, TURQUOISE);
	print("\trsi: ");
	itoa(regs[4], buffer);
	printc(buffer, TURQUOISE);
	print("\nrdi: ");
	itoa(regs[5], buffer);
	printc(buffer, TURQUOISE);
	print("\trbp: ");
	itoa(regs[6], buffer);
	printc(buffer, TURQUOISE);
	print("\tcs: ");
	itoa(regs[7], buffer);
	printc(buffer, TURQUOISE);
	print("\tr8: ");
	itoa(regs[8], buffer);
	printc(buffer, TURQUOISE);
	print("\tr9: ");
	itoa(regs[9], buffer);
	printc(buffer, TURQUOISE);
	print("\tr10: ");
	itoa(regs[10], buffer);
	printc(buffer, TURQUOISE);
	print("\nr11: ");
	itoa(regs[11], buffer);
	printc(buffer, TURQUOISE);
	print("\tr12: ");
	itoa(regs[12], buffer);
	printc(buffer, TURQUOISE);
	print("\tr13: ");
	itoa(regs[13], buffer);
	printc(buffer, TURQUOISE);
	print("\tr14: ");
	itoa(regs[15], buffer);
	printc(buffer, TURQUOISE);
	print("\tr15: ");
	itoa(regs[16], buffer);
	printc(buffer, TURQUOISE);
	print("\trflags: ");
	itoa(regs[17], buffer);
	printc(buffer, TURQUOISE);
}

void exceptionDispatcher(uint64_t exception, uint64_t regs[18]) {
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
    printRegisters(regs);
    timer_wait(5*18); //un seg son 18 ticks
    resetMain();
}