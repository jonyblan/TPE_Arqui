#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>

extern uint64_t resetMain();

void exceptionDispatcher(int exception);

#endif