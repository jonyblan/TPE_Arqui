#include <stdint.h>
#include "time.h"
#include "libasm.h"

extern void _hlt();

uint64_t ticksElapsed(){
    return callTicksElapsed();
}

void timerWait(uint64_t waitTicks){
    uint64_t start = ticksElapsed();
    while(ticksElapsed() - start < waitTicks){
    	_hlt();
    }
}