
#include <stdint.h>
extern void writeInt(char c);

void putc(char c){
	writeInt(c);
	return;
}