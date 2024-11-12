//TODO: parametros no implementados, borrar

#include "arquilib.h"
#include <stdint.h>
#include "libasm.h"

/**
 * Imprime lo recibido por entrada
 * argc: la cantidad de argumentos recibidos por entrada (separados por espacio)
 * argv: lo recibido por entrada, array de strings
 */
void echo(uint64_t argc, const char * argv[]){
    for(int i=0; i<argc; i++){
        puts(argv[i]);
        putChar(' ');
    }
    putChar('\n');
    //sys_exit(0);
}