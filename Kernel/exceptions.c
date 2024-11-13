#include "videoDriver.h"
#include "time.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

void printRegisters(uint64_t regs[18]){

	puts("Registers values:");
	print("rip: ");
	printc(itoa(regs[0]), TURQUOISE);
	print("\trax: ");
	printc(itoa(regs[1]), TURQUOISE);
	print("\trbx: ");
	printc(itoa(regs[2]), TURQUOISE);
	print("\trcx: ");
	printc(itoa(regs[3]), TURQUOISE);
	print("\trdx: ");
	printc(itoa(regs[4]), TURQUOISE);
	print("\trsi: ");
	printc(itoa(regs[5]), TURQUOISE);
	print("\nrdi: ");
	printc(itoa(regs[6]), TURQUOISE);
	print("\trbp: ");
	printc(itoa(regs[7]), TURQUOISE);
	print("\tcs: ");
	printc(itoa(regs[8]), TURQUOISE);
	print("\tr8: ");
	printc(itoa(regs[9]), TURQUOISE);
	print("\tr9: ");
	printc(itoa(regs[10]), TURQUOISE);
	print("\tr10");
	printc(itoa(regs[11]), TURQUOISE);
	print("\nr11: ");
	printc(itoa(regs[12]), TURQUOISE);
	print("\tr12: ");
	printc(itoa(regs[13]), TURQUOISE);
	print("\tr13: ");
	printc(itoa(regs[14]), TURQUOISE);
	print("\tr14: ");
	printc(itoa(regs[15]), TURQUOISE);
	print("\tr15: ");
	printc(itoa(regs[16]), TURQUOISE);
	print("\trflags: ");
	printc(itoa(regs[17]), TURQUOISE);
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