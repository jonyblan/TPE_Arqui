#ifndef _BEEP_DRIVER_H
#define _BEEP_DRIVER_H
#include <stdint.h>

void beep(uint32_t freq, int wait);
void noSound(void);
void play_sound(uint32_t freq);
void sleep(uint32_t ticksToWait);
#endif