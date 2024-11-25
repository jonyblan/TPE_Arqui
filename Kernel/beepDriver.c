#include "beepDriver.h"
#include <stdint.h>
#include "videoDriver.h"

#define PIT_CONTROL_PORT 0x43
#define PIT_CHANNEL_2 0x42
#define PC_SPEAKER_PORT 0x61

/*
IMPORTANTE:
Nada de esto esta testeado, en teoria hay que poner la flag
-device pcspk
en run.sh, se conecta el parlante, y listo
pero en las 3 maquinas nos dio un error que no existe pcspk
*/

void playSound(uint32_t frequency) {
    if (frequency == 0) return;

    // divisor for the desired frequency
    uint32_t divisor = 1193180 / frequency;

    // Set the PIT to mode 3 (square wave generator) for channel 2
    outb(PIT_CONTROL_PORT, 0xB6); // 0xB6 = Select channel 2, mode 3
    outb(PIT_CHANNEL_2, (uint8_t)(divisor & 0xFF)); // High
    outb(PIT_CHANNEL_2, (uint8_t)((divisor >> 8) & 0xFF)); // Low

    uint8_t tmp = inb(PC_SPEAKER_PORT);
    // se trabaja con los 2 ultimos bits
    if (!(tmp & 3)) { 
        outb(PC_SPEAKER_PORT, tmp | 3); 
    }
}

void stopSound() {
    uint8_t tmp = inb(PC_SPEAKER_PORT) & 0xFC; // 1111 1100
    outb(PC_SPEAKER_PORT, tmp);
}

void sleep(uint32_t milliseconds) {
    volatile uint32_t counter = 0;

    // A rough approximation of delay
    for (uint32_t i = 0; i < milliseconds * 100000; i++) {
        counter++;
    }
}

void beep(uint32_t freq, int wait){
    playSound(freq);
    sleep(wait);
    stopSound();
}

void beep_driver(){
	beep(440, 1000);
}
