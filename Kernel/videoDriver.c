#include "videoDriver.h"
#include <stdint.h>

/** videoDriver.c:
 * Contenido proporcionado por la cátedra
 */
struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

/** videoDriver.c:
 * Fin del contenido proporcionado por la cátedra
*/

static uint8_t cursorX = 0;
static uint8_t cursorY = 0;

struct coordinates{
    uint8_t row;
    uint8_t col;
};

uint32_t getPixel(uint64_t x, uint64_t y){
    //Basado en putPixel, funcion de la catedra
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    uint32_t toReturn;
    toReturn |= framebuffer[offset];
    toReturn |= framebuffer[offset+1] << 8;
    toReturn |= framebuffer[offset+2] << 16;
    return toReturn;
}

/**
 * Salta una linea y vuelve al principio de linea
 */
void newLine(){
    /*
     * //TODO: si me quedo sin espacio en pantalla, mover todo hacia arriba
    uint16_t width = VBE_mode_info->width;
    uint16_t height = VBE_mode_info->height;
    if(cursorY == height/CHARACTER_HEIGHT - 1){
        for(int i=0; i<width; i++){
            for(int j=0; j<height-CHARACTER_HEIGHT; j++){
                putPixel(getPixel(i+CHARACTER_HEIGHT, j+CHARACTER_HEIGHT), i, j);
            }
        }
        for(int k=0; k<width; k++){
            for(int l=0; l=(int)CHARACTER_HEIGHT; l++){
                putPixel(BLACK, k, l);
            }
        }
    }
     */
    cursorX = 0;
    cursorY++;
}

void erase(){
    //TODO: si se borra el ultimo caracter de la linea, sigue estando la linea al hacer enter
    uint16_t width = VBE_mode_info->width;
    //static uint8_t base = cursorY; con cursorY>base
    if(cursorX==0 && cursorY>0){
        cursorY--;
        cursorX=width/CHARACTER_WIDTH;
    }
    cursorX--;
    for (int i = 0; i < CHARACTER_HEIGHT; i++) {
        for (int j = 0; j < CHARACTER_WIDTH; j++) {
            putPixel(BLACK,cursorX * CHARACTER_WIDTH + j, cursorY * CHARACTER_HEIGHT + i);
        }
    }
}

/**
 * Recibe un caracter ASCII y obtiene la coordenada correspondiente al primer pixel en font_bitmap
 * @param character caracter en ASCII
 * @return par de coordenadas
 */
struct coordinates charToCoord(char character){
    uint8_t row;
    uint8_t col;
    if(character >= '0' && character <= '9'){
        row = 1;
        col = character - '0';
    }
    if(character >= 'A' && character <= 'Z'){
        if(character<='O'){
            row = 2;
            col = character - 'A' + 1;
        } else {
            row = 3;
            col = character - 'P';
        }
    }
    if(character >= 'a' && character <= 'z'){
        if(character <= 'o'){
            row = 4;
            col = character - 'a' + 1;
        } else {
            row = 5;
            col = character - 'p';
        }
    }
    switch(character){
        case ' ': row = 0; col = 0; break;
        case '!': row = 0; col = 1; break;
        case '"': row = 0; col = 2; break;
        case '#': row = 0; col = 3; break;
        case '$': row = 0; col = 4; break;
        case '%': row = 0; col = 5; break;
        case '&': row = 0; col = 6; break;
        case '\'': row = 0; col = 7; break;
        case '(': row = 0; col = 8; break;
        case ')': row = 0; col = 9; break;
        case '*': row = 0; col = 10; break;
        case '+': row = 0; col = 11; break;
        case ',': row = 0; col = 12; break;
        case '-': row = 0; col = 13; break;
        case '.': row = 0; col = 14; break;
        case '/': row = 0; col = 15; break;

        case ':': row = 1; col = 10; break;
        case ';': row = 1; col = 11; break;
        case '<': row = 1; col = 12; break;
        case '=': row = 1; col = 13; break;
        case '>': row = 1; col = 14; break;
        case '?': row = 1; col = 15; break;

        case '@': row = 2; col = 0; break;

        case '[': row = 3; col = 11; break;
        case '\\': row = 3; col = 12; break;
        case ']': row = 3; col = 13; break;
        case '^': row = 3; col = 14; break;
        case '_': row = 3; col = 15; break;

        case '`': row = 4; col = 0; break;

        case '{': row = 5; col = 11; break;
        case '|': row = 5; col = 12; break;
        case '}': row = 5; col = 13; break;
        case '~': row = 5; col = 14; break;
        //default: row = 0; col = 0; break;   //por defecto, imprime espacio en blanco, no usar
    }
    struct coordinates coord;
    coord.row = row*CHARACTER_HEIGHT;
    coord.col = col*CHARACTER_WIDTH;
    return coord;
}

/**
 * Imprime el caracter recibido de font_bitmap con formato y salta al siguiente espacio.
 * @param row fila del caracter en font_bitmap
 * @param col columna del caracter en font_bitmap
 * @param hexColor color en hexa 32b
 */
void putCharCoordf(struct coordinates coord, uint32_t hexColor) {
    for (int i = 0; i < CHARACTER_HEIGHT; i++) {
        for (int j = 0; j < CHARACTER_WIDTH; j++) {
            putPixel(hexColor & (font_bitmap[i + coord.row][j + coord.col] != 0 ? WHITE : BLACK),
                     cursorX * CHARACTER_WIDTH + j, cursorY * CHARACTER_HEIGHT + i);
        }
    }
    cursorX++;
}

/**
 * Mismo funcionamiento, imprime en blanco
 * @param row
 * @param col
 */
void putCharCoord(struct coordinates coord){
    putCharCoordf(coord, WHITE);
}

/**
 * Imprime un caracter con formato y salta de linea
 * @param character caracter a imprimir
 * @param hexColor formato
 */
void putCharf(char character, uint32_t hexColor){
    uint16_t width = VBE_mode_info->width;
    if(character == '\n' || cursorX==width/CHARACTER_WIDTH){
        newLine();
    }
    if(character == '\b'){
        erase();
    }
    if(character != '\n' && character != '\b') { //alternativa: array de "nonprintable chars"
        struct coordinates coord = charToCoord(character);
        putCharCoordf(coord, hexColor);
    }
}

void putChar(char character){
    putCharf(character, WHITE);
}

/*
 * Imprime string con color sin saltos
 */
void printc(char * string, uint32_t hexColor){
    uint16_t width = VBE_mode_info->width;

    for(int i=0; string[i]!=0; i++){
        if(cursorX==width/CHARACTER_WIDTH){
            newLine();
        }
        putCharf(string[i], hexColor);
    }
}

void print(char * string){
    printc(string, WHITE);
}

/**
 * Imprime un string con formato
 * @param string
 * @param hexColor
 */
void putsf(char * string, uint32_t hexColor){
    printc(string, hexColor);
    newLine();
}

/**
 * Imprime un string
 * @param string
 */
void puts(char * string){
    char * aux = string;
    putsf(aux, WHITE);
}

/**
 * Imprime un caracter en una coordenada especifica de la pantalla. No avanza.
 * @param row
 * @param col
 * @param hexColor
 * @param x
 * @param y
 */
void putCharScreen(uint8_t row, uint8_t col, uint32_t hexColor, uint64_t x, uint64_t y){
    for (int i=0; i<CHARACTER_HEIGHT; i++){
        for(int j=0; j<CHARACTER_WIDTH; j++){
            putPixel(hexColor & (font_bitmap[i+row][j+col] != 0 ? WHITE: BLACK), x+j, y+i);
        }
    }
}

/**
 * Limpia la pantalla
 */
void clear(){
    for(int i=0; i<VBE_mode_info->width; i++){
        for(int j=0; j<VBE_mode_info->height; j++){
            putPixel(BLACK, i, j);
        }
    }
    cursorX=0;
    cursorY=0;
}