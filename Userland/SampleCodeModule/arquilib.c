#include <stdint.h>
#include "include/arquilib.h"
#include "libasm.h"

void putChar(const char c){
	/*
    writeInt(c);
	return;
	 */
    sys_write(STANDARD_OUTPUT, &c, 1);
}

//TODO: imprimir caracter con color
/*
void putCharf(const char c, uint32_t hexColor){

}
 */

uint64_t strlen(const char * string){
    uint64_t ret = 0;
    while(string[ret]!='\0'){
        ret++;
    }
    return ret;
}

void puts(const char * string){
    sys_write(STANDARD_OUTPUT, string, strlen(string));
}

char getChar(){
	/*
    writeInt('a');
	char c = (char)readInt();
	return c;
	 */
    char c;
    sys_read(STANDARD_INPUT, &c, 1);
    return c;
}

//TODO: scan (leer mas de un char)


extern void printDateTimeInt();
extern int getTicksInt();

void printDateTime(){
	printDateTimeInt();
}

int getTicks(){
	return getTicksInt();
}