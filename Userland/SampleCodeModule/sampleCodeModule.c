/* sampleCodeModule.c */

#include "arquilib.h"
#include <stdint.h>

int shell() {
	putChar('0');
	printDateTime();
	putChar('0' + (char)(getTicks() % 10));
	char aux = getChar();
	putChar(aux);
	int a;
	while(1){
		a = getTicks();
		if(a==5){
			putChar('5');
		}
	}
	return 0;
}