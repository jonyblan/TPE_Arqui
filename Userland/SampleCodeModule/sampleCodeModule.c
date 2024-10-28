/* sampleCodeModule.c */

#include "arquilib.h"
#include <stdint.h>

void putc(char c);

int shell() {
	putc('@');
	return 0;
}