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
	ncPrintln("Registers values:");
	ncPrint("rax: ");
	ncPrint(numToString64(regs.rax));
	ncPrintln("");
	ncPrint("rbx: ");
	ncPrint(numToString64(regs.rbx));
	ncPrintln("");
	ncPrint("rcx: ");
	ncPrint(numToString64(regs.rcx));
	ncPrintln("");
	ncPrint("rdx: ");
	ncPrint(numToString64(regs.rdx));
	ncPrintln("");
	ncPrint("r8: ");
	ncPrint(numToString64(regs.r8));
	ncPrintln("");
	ncPrint("r9: ");
	ncPrint(numToString64(regs.r9));
	ncPrintln("");
	ncPrint("r10: ");
	ncPrint(numToString64(regs.r10));
	ncPrintln("");
	ncPrint("r11: ");
	ncPrint(numToString64(regs.r11));
	ncPrintln("");
	ncPrint("r12: ");
	ncPrint(numToString64(regs.r12));
	ncPrintln("");
	ncPrint("r13: ");
	ncPrint(numToString64(regs.r13));
	ncPrintln("");
	ncPrint("r14: ");
	ncPrint(numToString64(regs.r14));
	ncPrintln("");
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