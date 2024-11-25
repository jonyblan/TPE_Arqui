#include "arquilib.h"

/**
 * Muestra los comandos disponibles y una breve descripcion de ellos
 */
void help(){
    printc("clear: limpia la pantalla\n", ORANGE);
    printc("scale {1-4}: cambia la escala del texto\n", ORANGE);
    printc("div0: causa una interrupcion de division por 0\n", ORANGE);
    printc("invopcode: causa una interrupcion de Invalid Operation Code\n", ORANGE);
    printc("printregs: imprime los valores actuales de los registros del procesador\n", ORANGE);
    printc("time: imprime la hora del sistema\n", ORANGE);
    printc("snake: inicia el juego snake para dos jugadores\n", ORANGE);
    printc("8ball {msg}: hacele una pregunta a la bola 8\n", ORANGE);
}