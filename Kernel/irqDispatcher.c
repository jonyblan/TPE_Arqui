#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <naiveConsole.h>

static void int_20();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
	}
	return;
}

void syscallDispatcher(uint64_t rax, uint64_t rdi, int rsi) {
	putChar(32, 24);
	switch (rax) {
		case 0:
			//read();
			break;
		case 1:
			//write();
			//uint8_t aux = '@';
			char aux = (char)rdi;
			putChar(32, aux/2);
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
