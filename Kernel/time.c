#include "time.h"
#include <stdint.h>
#include "interrupts.h"

extern uint8_t getSecs();
extern uint8_t getMins();
extern uint8_t getHours();
extern uint8_t getDay();
extern uint8_t getMonth();
extern uint8_t getYear();

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

uint64_t get_sys_time(){
    //ajusto a GMT-3
    uint8_t hours = getHours();
    if(hours >= 3){
        hours -= 3;
    } else {
        hours = 24 - 3 + hours;
    }
    //formato: YYMMDDhhmmss
    return getSecs() + getMins() * 100 + hours * 10000 + getDay() * 1000000 + getMonth() * 100000000 + getYear() * 10000000000;
}