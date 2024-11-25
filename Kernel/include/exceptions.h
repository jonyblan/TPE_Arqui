#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>

#define REGS_COUNT 18
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

extern uint64_t resetMain();

void exceptionDispatcher(uint64_t exception, uint64_t regs[REGS_COUNT]);

#endif