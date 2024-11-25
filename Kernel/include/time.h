#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include "interrupts.h"

#define TICKS_PER_SECOND 18

extern uint8_t getSecs();
extern uint8_t getMins();
extern uint8_t getHours();
extern uint8_t getDay();
extern uint8_t getMonth();
extern uint8_t getYear();

void timer_handler();
uint64_t ticks_elapsed();
uint64_t seconds_elapsed();
void timer_wait(uint64_t waitTime);
uint64_t get_sys_time();

#endif
