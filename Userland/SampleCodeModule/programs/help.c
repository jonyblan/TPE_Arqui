#include "arquilib.h"

/**
 * Muestra los comandos disponibles y una breve descripcion de ellos
 */
void help(){
    printc("clear: limpia la pantalla\n", ORANGE);
<<<<<<< Updated upstream
=======
    printc("scale {1-4}: cambia la escala del texto\n", ORANGE);
>>>>>>> Stashed changes
    printc("div0: causa una interrupcion de division por 0\n", ORANGE);
    printc("invopcode: causa una interrupcion de Invalid Operation Code\n", ORANGE);
    printc("printregs: imprime los valores actuales de los registros del procesador\n", ORANGE);
}