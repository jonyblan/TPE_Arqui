#include "videoDriver.h"
#include "time.h"
#include "printRegisters.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

void exceptionDispatcher(uint64_t exception, uint64_t regs[18]) {
    clear();
    switch(exception) {
        case ZERO_EXCEPTION_ID:
            printc("Exception 0: Division by zero", RED);
            break;
        case INVALID_OPCODE_ID:
            printc("Exception 6: Invalid operation code", RED);
            break;
        default:
            printc("Uncontrolled exception", RED);
    }
    printRegisters(regs);
}