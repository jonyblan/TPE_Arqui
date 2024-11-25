#ifndef PRINTREGISTERS_H
#define PRINTREGISTERS_H

#include <stdint.h>
#include "time.h"
#include "videoDriver.h"

#define REGISTER_COUNT 18

void printRegisters(uint64_t regs[REGISTER_COUNT]);

#endif
