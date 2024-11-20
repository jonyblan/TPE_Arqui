#include "arquilib.h"
#include <stdint.h>
#include "programs.h"
#include "utillib.h"
#include "libasm.h"

#define TEMP_BUFFER_SIZE 256

static const char * functions[]={ "exit", "help", "clear", "scale", "div0", "invopcode", 
                                  "printregs", "time", "snake"/*, "snake2"*/ };

static const uint16_t functionCount = sizeof(functions);

static uint8_t exit = 0;

void run(const char *);
void shellLoop();

void shell(uint64_t argc, const char * argv[]){
    printc("Shell initialized, welcome to gordOS\n", LINUX_BLUE);
    if(exit){
        return;
    }
    shellLoop();
    exit=0;
}

void shellLoop(){
    while(!exit){
        printc("user@gordOS", LINUX_GREEN);
        printc("$ ", WHITE);

        uint64_t i = 0;
        char buffer[TEMP_BUFFER_SIZE] = {0};
        char c = 0;

        while((c=getChar())!='\n' && c >= 0 && i + 1 < TEMP_BUFFER_SIZE){

            if(c!=0) {
                if(c == '\b' && i > 0){
                    i--;
                    buffer[i]=0;
                    putChar(c);
                } else if(c!='\b') { //para que no borre si no hay nada en buffer
                    buffer[i++] = c;
                    putChar(c);
                }
            }
        }
	if(buffer[0] != 0){
        	run(buffer);
	}
    }
}

void run(const char * buffer){

    uint8_t functionId = 0;
    char found = 0;
    char str[TEMP_BUFFER_SIZE];
    strcpyUntil(str, buffer, ' ');
    for(uint8_t i=0; i<functionCount && !found; i++){
        if(strcmp(str, functions[i]) == 0){
            found = 1;
            functionId = i;
        }
    }

    //sobreescribo el nombre de la funcion, ya la encontre
    strcpyFrom(str, buffer, ' ');
    switch(functionId){
        case 0: exit=1; break; //exit()
        case 1: help(); break;
        case 2: clear(); break;
        case 3: scale(str); break;
        case 4: excDiv0(); break;
        case 5: excInvalidOpCode(); break;
        case 6: callPrintRegs(); break;
        case 7: callPrintSystemTime(); break;
        case 8: snake(); break;
        //case 9: snake2(); break;
        default: printc("Command not found\n", RED);
    }
}