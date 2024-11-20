#include "keyboard.h"
#include <stdint.h>
#include "videoDriver.h"
#include <time.h>
#include "mouse.h"

static void int_20();
static void int_21();
static void int_21();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
		case 12:
			int_32();
			break;
	}
    return;
}

void int_20() {
	timer_handler();
}

void int_21() {
    key_handler();
}

void int_32() {
	mouse_handler();
}