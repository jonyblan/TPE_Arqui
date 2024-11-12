//todo: exit y shutdown

#include "arquilib.h"
#include <stdint.h>
#include "programs.h"

#define TEMP_BUFFER_SIZE 256
#define TEMP_MAX_PARAM 16

typedef uint64_t (*Program)(uint64_t, const char * []);

typedef struct {
    const char * progName;
    Program prog;
} ProgramByName;

static const ProgramByName programs[] = {
        {"echo", (Program)echo}

}; //todo: esto
static const uint64_t programCount = sizeof(programs);

static uint8_t exit = 0;
static uint8_t shutdown = 0;

void run(const char *, uint64_t);
void shellLoop();

void shell(uint64_t argc, const char * argv[]){
    puts("Bienvenido");
    shellLoop();
    exit=0;
    sys_exit(exit);
}

void shellLoop(){
    while(!exit && !shutdown){
        puts("shell");

        uint64_t i = 0;
        char buffer[TEMP_BUFFER_SIZE] = {0};
        char c = 0;

        while((c=getChar())!='\n' && c >= 0 && i + 1 < TEMP_BUFFER_SIZE){
            //puts("loop");
            //TODO: borrar
            //TODO: fin del buffer
            if(c!=0) {
                if(c == '\b' && i > 0){
                    puts("borrar");
                    //todo: borrar (buffer[i] = 0, decrementar i, borrar el caracter (se puede imprimir \b)
                } else {
                    //puts("buffer");
                    buffer[i++] = c;
                    putChar(c);
                }
            }
        }
        //putChar('\n');    //comentado porque imprime el enter que escribe el usuario
        /* modelo de uso de putPixel
        for(int index=64; index<128; index++){
            putPixel(0x0000FF00, index, 64);
        }
         */
        run(buffer, i);
    }
}

void run(const char * buffer, uint64_t bufferLength){
    puts("run");
    char prog[TEMP_BUFFER_SIZE];
    char progArgv[TEMP_MAX_PARAM][TEMP_BUFFER_SIZE];
    uint8_t spaceCount = 0;
    uint64_t j=0;
    for(uint64_t i=0; i<bufferLength; i++){
        if(buffer[i]!=' '){
            if(spaceCount==0){
                prog[i]=buffer[i];
            } else {
                progArgv[spaceCount-1][j++]=buffer[i];
            }
        } else {
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
}