#ifndef _BEEP_DRIVER_H
#define _BEEP_DRIVER_H
#include <stdint.h>

extern void outb(uint16_t arg1, uint16_t arg2);
extern uint16_t inb(uint16_t arg);

void beep(uint32_t freq, int wait);
void noSound(void);
void play_sound(uint32_t freq);
void sleep(uint32_t ticksToWait);
void beep_driver();
#endif