#include <beepDriver.h>
#include <stdint.h>

void noSound(void){
    uint8_t tmp = inb(0x61) & 0xFC; // Disable sound
    outb(0x61, tmp);
}

void play_sound(uint32_t freq){
    uint32_t divisor = 1193180 / freq;
    outb(0x43, 0xb6);
	outb(0x42, (uint8_t)(divisor));
	outb(0x42, (uint8_t)(divisor >> 8));
 // Set high byte

    // Enable the speaker
    uint8_t tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

void sleep(uint32_t ticksToWait)
{
	int ticksAtCall = ticks_elapsed();
	while (ticks_elapsed() - ticksAtCall< ticksToWait);
    return;
}

void beep(uint32_t freq, int wait){
    play_sound(freq);
    sleep(wait);
    noSound();
}

