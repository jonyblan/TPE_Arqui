//todo: exit y shutdown

#include "arquilib.h"
#include <stdint.h>
#include "programs.h"
#include "utillib.h"
#include "libasm.h"

#define TEMP_BUFFER_SIZE 256
#define TEMP_MAX_PARAM 5

/*
 * TODO: parametros
typedef uint64_t (*Program)(uint64_t, const char * []);

typedef struct {
    const char * progName;
    Program prog;
} ProgramByName;

static const ProgramByName programs[] = {
        {"echo", (Program)echo}

};
static const uint64_t programCount = sizeof(programs);
 */

static const char * functions[]={ "exit", "help", "clear", "div0", "invopcode", "printregs" };

static const uint16_t functionCount = sizeof(functions);

static uint8_t exit = 0;
//static uint8_t shutdown = 0;

void run(const char *);
void shellLoop();

void shell(uint64_t argc, const char * argv[]){
    printc("Shell initialized, welcome to gordOS\n", LINUX_BLUE);
    shellLoop();
    exit=0;
    //sys_exit(exit);
}

void shellLoop(){
    while(!exit/* && !shutdown*/){
        printc("user@gordOS", LINUX_GREEN);
        printc("$ ", WHITE);

        uint64_t i = 0;
        char buffer[TEMP_BUFFER_SIZE] = {0};
        char c = 0;

        while((c=getChar())!='\n' && c >= 0 && i + 1 < TEMP_BUFFER_SIZE){

            //TODO: fin del buffer
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

        /* modelo de uso de putPixel TODO borrar
        for(int index=64; index<128; index++){
            putPixel(0x0000FF00, index, 64);
        }
         */

        run(buffer);

        //shellLoop(); //TODO no se si usarlo
    }
}

void run(const char * buffer){

    uint8_t functionId = 0;
    char found = 0;
    for(uint8_t i=0; i<functionCount && !found; i++){
        if(strcmp(buffer, functions[i]) == 0){
            found = 1;
            functionId = i;
        }
    }

    switch(functionId){
        //case 0: exit=1; break; //exit()
        case 1: help(); break;
        case 2: clear(); break;
        case 3: excDiv0(); break;
        case 4: excInvalidOpCode(); break;
        case 5: callPrintRegs(); break;
        default: //TODO: arrojar excepcion 6!!!!
    }

    /*
    char prog[TEMP_BUFFER_SIZE] = {0};
    char progArgv[TEMP_MAX_PARAM][TEMP_BUFFER_SIZE] = {0};
    uint8_t spaceCount = 0;
    uint64_t argvIndex=0;
    for(uint64_t i=0; i<bufferLength; i++){
        if(buffer[i]!=' '){
            if(spaceCount==0){
                prog[i]=buffer[i];
            } else {
                progArgv[spaceCount-1][argvIndex++]=buffer[i];
            }
        } else {
            if(spaceCount==0){
                prog[i] = 0;
            } else {
                progArgv[spaceCount-1][argvIndex] = 0;
            }
            spaceCount++;
            //todo: no chequea doble espacio
        }
    }
    //busco el programa
    uint8_t found = 0;
    for(uint8_t k=0; k<programCount; k++){
        if(strcmp(prog, programs[k].progName)==0){
            found = 1;
            sys_execve(programs[k], spaceCount, progArgv);
        }
    }
    if(found == 0){
        //todo: tirar exc 6
    }
     */
}