#ifndef _TIME_H_
#define _TIME_H_
#include <stdint.h>

void timer_handler();
uint64_t ticks_elapsed();
uint64_t seconds_elapsed();

#endif
