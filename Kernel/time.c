#include "time.h"
#include <stdint.h>
#include "interrupts.h"

static uint64_t ticks = 0;

void timer_handler() {
	ticks++;
}

uint64_t ticks_elapsed() {
	return ticks;
}

uint64_t seconds_elapsed() {
	return ticks / 18;
}

void timer_wait(uint64_t waitTime){
	uint64_t start = ticks;
    while(ticks - start < waitTime){
    	_hlt();
    }
}