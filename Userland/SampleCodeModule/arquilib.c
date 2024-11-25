#include "arquilib.h"

/**
 * Imprime un caracter con formato y salta al siguiente espacio
 * @param character caracter ASCII a imprimir
 * @param hexColor color 0x00RRGGBB
 */
void putCharc(const char c, uint32_t hexColor){
    printc(&c, hexColor);
}

/**
 * Imprime un caracter en blanco y salta al siguiente espacio
 * @param character caracter ASCII a imprimir
 */
void putChar(const char c){
    putCharc(c, WHITE);
}

/**
 * Imprime string con color sin salto de linea
 * @param string string a imprimir
 * @param hexColor color 0x00RRGGBB
 */
void printc(const char * string, uint32_t hexColor){
    sysWriteColor(STANDARD_OUTPUT, string, strlen(string), hexColor);
}

/**
 * Imprime string en blanco sin salto de linea
 * @param string string a imprimir
 */
void print(const char * string){
    printc(string, WHITE);
}

/**
 * Imprime string con color con salto de linea
 * @param string string a imprimir
 * @param hexColor color 0x00RRGGBB
 */
void putsc(const char * string, uint32_t hexColor){
    printc(string, hexColor);
    callNewLine();
}

/**
 * Imprime string en blanco con salto de linea
 * @param string string a imprimir
 */
void puts(const char * string){
    putsc(string, WHITE);
    callNewLine();
}

/**
 * Lee el caracter ingresado por entrada estandar
 * @return caracter ingresado
 */
char getChar(){
    return sysReadChar(STANDARD_INPUT);
}

/**
 * Imprime un pixel en una coordenada de la pantalla
 * @param hexColor color del pixel en formato 0x00RRGGBB
 * @param x coordenada x
 * @param y coordenada y
 */
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y){
    callPutPixel(hexColor, x, y);
}

/**
 * Limpia la pantalla
 */
void clear(){
    callClear();
}

/**
 * Cambia la escala de los caracteres.
 * @param argv escala a la que cambiar [1-4]
 */
void scale(char * argv){
    if(strlen(argv) != 1){
        printc("Illegal argument\n", RED);
    } else {
        callScale(argv[0]-'0');
    }
}