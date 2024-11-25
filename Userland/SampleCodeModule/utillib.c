#include "utillib.h"

/**
 * Basada en la libreria estandar de C, devuelve la cantidad de chars de un string
 * @param string string a contar chars
 * @return cantidad de chars
 */
uint64_t strlen(const char * string){
    uint64_t ret = 0;
    while(string[ret]!=0){
        ret++;
    }
    return ret;
}

/**
 * Basada en la libreria estandar de C, compara dos strings
 * @param str1 string a comparar
 * @param str2 string por el que comparar
 * @return 0 si son iguales, 1 si str1 es "mayor", -1 si str2 es "mayor"
 */
int8_t strcmp(const char* str1, const char* str2){
    int8_t cmp = 0;
    for(int i = 0; str1[i]!=0 && str2[i]!=0 && cmp == 0; i++){
        if(str1[i+1] == 0 && str2[i+1] != 0) {cmp = -1;}
        else if(str1[i+1] != 0 && str2[i+1] == 0) {cmp = 1;}
        else { cmp = str1[i]-str2[i]; }
    }
    return cmp;
}

/**
 * Inspirada en la libreria estandar de C, copia un string hasta un caracter definido
 * @param dest string donde pegar
 * @param src string del que copiar
 * @param stop caracter hasta el cual copiar
 */
void strcpyUntil(char * dest, const char * src, char stop){
    if(src == 0 || src[0] == 0){
        dest[0] = 0;
    }

    int i = 0;
    for(; src[i]!=0 && src[i]!=stop; i++){
        dest[i] = src[i];
    }
    dest[i] = 0;
}

/**
 * Inspirada en la libreria estandar de C, copia un string desde un caracter definido
 * @param dest string donde pegar
 * @param src string del que copiar
 * @param start caracter desde el cual copiar
 */
void strcpyFrom(char * dest, const char * src, char start){
    if(src == 0 || src[0] == 0){
        dest[0] = 0;
    }

    int i = 0;
    while(src[i]!=start){
        i++;
    }
    i++;
    int j = 0;
    for(; src[i]!=0; i++, j++){
        dest[j] = src[i];
    }
    dest[j] = 0;
}