#include "videoDriver.h"
#include "time.h"
#include "printRegisters.h"
#include "exceptions.h"

void exceptionDispatcher(uint64_t exception, uint64_t regs[REGS_COUNT]) {
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
    timer_wait(10*TICKS_PER_SECOND);
    resetMain();
}