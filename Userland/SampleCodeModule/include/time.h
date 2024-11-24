#ifndef USERLAND_TIME_H
#define USERLAND_TIME_H
#include <stdint.h>

    uint64_t ticksElapsed();
    void timerWait(uint64_t waitTicks);
    
#endif