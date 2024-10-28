
#include <stdint.h>
extern void writeInt(char c);
extern uint64_t readInt();
extern void printDateTimeInt();
extern int getTicksInt();

void putChar(char c){
	writeInt(c);
	return;
}

char getChar(){
	//writeInt('a');
	char c = (char)readInt();
	return c;
}

void printDateTime(){
	printDateTimeInt();
}

int getTicks(){
	return getTicksInt();
}