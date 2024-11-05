
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

void puts(char * s){
    for(int i = 0; s[i]!='\0'; i++){
        putChar(s[i]);
    }
    return;
}