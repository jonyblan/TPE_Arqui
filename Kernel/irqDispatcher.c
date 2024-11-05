#include "keyboard.h"
#include <stdint.h>
#include "videoDriver.h"
#include <time.h>

static void int_20();
static void int_21();

static int waiting = 0;

void irqDispatcher(uint64_t irq) {
	putsf("In irqDispatcher()", YELLOW);
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
	}
    putsf("Out irqDispatcher()", YELLOW);
    return;
}

char readChar(){
	return key_handler();
}

void auxChar(){
	if(waiting == 1){
		putChar('c');
		waiting = 2;
	}
}

uint64_t syscallDispatcher(uint64_t rax, uint64_t rdi, int rsi) {
	switch (rax) {
		case 0:
			//return (uint64_t)'r';
			waiting = 1;
			//putChar('d');
			while(waiting != 2){
			
			}
			//putChar('e');
			return (uint64_t)readChar();
			break;
		case 1:
			//write();
			char aux = (char)rdi;
			putChar(aux);
			break;
		case 2:
			printDateTime();
			break;
		case 3:
			return ticks_elapsed();
			break;
	}
	return 0;
}

void int_20() {
	timer_handler();
}

void int_21() {
	//putChar('b');
	auxChar();
    //deberia llamar a key_handler()
}
