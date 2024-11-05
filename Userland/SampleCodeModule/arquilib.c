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

int8_t strcmp(const char* str1, const char* str2){
    int8_t cmp = 0;
    for(int i = 0; str1[i]!='\0' && str2[i]!='\0' && cmp == 0; i++){
        if(str1[i+1] == '\0' && str2[i+1] != '\0') {cmp = -1;}
        if(str1[i+1] != '\0' && str2[i+1] == '\0') {cmp = 1;}
        else { cmp = str1[i]-str2[i]; }
    }
    return cmp;
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

/*
void puts(char * s){
    for(int i = 0; s[i]!='\0'; i++){
        putChar(s[i]);
    }
    return;
}*/