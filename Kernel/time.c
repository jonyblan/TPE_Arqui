#include "time.h"

static uint64_t ticks = 0;

void timer_handler() {
	ticks++;
}

uint64_t ticks_elapsed() {
	return ticks;
}

uint64_t seconds_elapsed() {
	return ticks / TICKS_PER_SECOND;
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