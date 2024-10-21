#include <time.h>
#include <naiveConsole.h>

int8_t key_pressed();

static unsigned long ticks = 0;

char getInput(){
	int8_t num = key_pressed();
	char aux = (char)num;
	char c = getCharacter(num);
}

void key_handler() {
	char c = getInput();
	ncPrintChar(c);
}