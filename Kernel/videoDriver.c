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
static uint8_t scale = 2;    //1 es para caracteres de 8*16 pix

#define SCALED_CHARACTER_WIDTH (CHARACTER_WIDTH * scale)
#define SCALED_CHARACTER_HEIGHT (CHARACTER_HEIGHT * scale)
#define WIDTH_IN_CHARS (VBE_mode_info->width / SCALED_CHARACTER_WIDTH)
#define HEIGHT_IN_CHARS (VBE_mode_info->height / SCALED_CHARACTER_HEIGHT)

struct coordinates{
    uint8_t row;
    uint8_t col;
};

/**
 * Cambia la escala de los caracteres.
 * 1: 100% (original)
 * 2: 200%
 * 3: 300%
 * 4: 400%
 */
void setScale(uint8_t newScale){
    if(scale >= 1 && scale <= 4)
        scale = newScale;
}

void upscale(){
    setScale(scale + 1);
}

void downscale(){
    setScale(scale - 1);
}

/**
 * Setea el cursor a una coordenada particular, respetando el tamaño de los chars
 * @param x coordenada en x
 * @param y coordenada en y
 */
void setCursor(uint8_t x, uint8_t y) {
    if(x < WIDTH_IN_CHARS){
        cursorX = x;
    } else {
        cursorX = WIDTH_IN_CHARS;
    }
    if(y < HEIGHT_IN_CHARS){
        cursorY = y;
    } else {
        cursorY = HEIGHT_IN_CHARS;
    }
}

/**
* Retorna el color del pixel de las coordenadas de pantalla x, y
* Basado en putPixel, funcion de la catedra
*/
uint32_t getPixel(uint64_t x, uint64_t y){
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    uint32_t toReturn;
    toReturn |= framebuffer[offset];
    toReturn |= framebuffer[offset+1] << 8;
    toReturn |= framebuffer[offset+2] << 16;
    return toReturn;
}

/**
 * Salta una linea y vuelve al principio de linea. Si se termina la pantalla, scrollea
 */
void newLine(){
    cursorX = 0;
    if(cursorY < HEIGHT_IN_CHARS-2) {
        cursorY++;          //avanzo normalmente, sigo teniendo pantalla
    } else {                //si llegue a la anteultima linea
        //como la pantalla esta en memoria, voy a copiar a partir de la segunda linea y pegarlo todo de vuelta
        //en el framebuffer. luego limpio la ultima linea para volver a escribir
        uint32_t framebuffer = VBE_mode_info->framebuffer;
        uint16_t height = VBE_mode_info->height;
        uint16_t pitch = VBE_mode_info->pitch;
        /*
         * memcpy(destino, origen, bytes)
         * destino: framebuffer (inicio de la pantalla)
         * origen: segunda linea de la pantalla (pitch es bytes por linea de pixeles)
         * bytes: toda la pantalla menos las ultimas dos lineas
        */
        memcpy(framebuffer,
               framebuffer + pitch * SCALED_CHARACTER_HEIGHT,
               pitch * (height - SCALED_CHARACTER_HEIGHT*2));
        /*
         * memset(destino, valor, bytes)
         * destino: anteultima linea
         * bytes: toda la linea
         */
        memset(framebuffer + pitch * (height - SCALED_CHARACTER_HEIGHT*2), 0, pitch * SCALED_CHARACTER_HEIGHT);
    }
}

void erase(){
    uint16_t width = VBE_mode_info->width;
    if(cursorX==0 && cursorY>0){
        cursorY--;
        cursorX=WIDTH_IN_CHARS;
    }
    cursorX--;
    for (int i = 0; i < SCALED_CHARACTER_HEIGHT; i++) {
        for (int j = 0; j < SCALED_CHARACTER_WIDTH; j++) {
            putPixel(BLACK,cursorX * SCALED_CHARACTER_WIDTH + j, cursorY * SCALED_CHARACTER_HEIGHT + i);
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
        //default: row = 0; col = 0; break;   //no usar
    }
    struct coordinates coord;
    coord.row = row*CHARACTER_HEIGHT;
    coord.col = col*CHARACTER_WIDTH;
    return coord;
}

/**
 * Imprime un caracter con formato y salta al siguiente espacio
 * @param character caracter ASCII a imprimir
 * @param hexColor color 0x00RRGGBB
 */
void putCharc(char character, uint32_t hexColor){
    if(cursorX==WIDTH_IN_CHARS){
        newLine();
    }

    switch(character){
        case '\n':
            newLine();
            break;
        case '\b':
            erase();
            break;
        case '\t':
            print("    ");
            break;
        default:
            struct coordinates coord = charToCoord(character);
            for (int i = 0; i < SCALED_CHARACTER_HEIGHT; i++) {
                for (int j = 0; j < SCALED_CHARACTER_WIDTH; j++) {
                    putPixel(hexColor & (font_bitmap[i/scale + coord.row][j/scale + coord.col] != 0 ? WHITE : BLACK),
                         cursorX * SCALED_CHARACTER_WIDTH + j, cursorY * SCALED_CHARACTER_HEIGHT + i);
                }
            }
            cursorX++;
    }
}

/**
 * Imprime un caracter en blanco y salta al siguiente espacio
 * @param character caracter ASCII a imprimir
 */
void putChar(char character){
    putCharc(character, WHITE);
}

/*
 * Imprime string con color sin salto de linea
 * @param string string a imprimir
 * @param hexColor color 0x00RRGGBB
 */
void printc(char * string, uint32_t hexColor){
    uint16_t width = VBE_mode_info->width;
    for(int i=0; string[i]!=0; i++){
        if(cursorX==WIDTH_IN_CHARS){
            newLine();
        }
        putCharc(string[i], hexColor);
    }
}

void print(char * string){
    printc(string, WHITE);
}

/*
 * Imprime string con color con salto de linea
 * @param string string a imprimir
 * @param hexColor color 0x00RRGGBB
 */
void putsc(char * string, uint32_t hexColor){
    printc(string, hexColor);
    newLine();
}

/*
 * Imprime string en blanco con salto de linea
 * @param string string a imprimir
 */
void puts(char * string){
    char * aux = string;
    putsc(aux, WHITE);
}

/**
 * Imprime un caracter en una coordenada especifica de la pantalla, no avanza el cursor
 * @param row
 * @param col
 * @param hexColor
 * @param x
 * @param y
 */
void putCharScreen(uint8_t row, uint8_t col, uint32_t hexColor, uint64_t x, uint64_t y){
    for (int i=0; i<SCALED_CHARACTER_HEIGHT; i++){
        for(int j=0; j<SCALED_CHARACTER_WIDTH; j++){
            putPixel(hexColor & (font_bitmap[i/scale+row][j/scale+col] != 0 ? WHITE: BLACK), x+j, y+i);
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

void itoa(long num, char * string){
    char isneg = 0;
    uint8_t i = 0;

    if(num==0){
        string[i++] = '0';
        string[i] = '\0';
        return;
    }
    if(num<0){
        num-=num;
        isneg = 1;
    }
    while(num != 0){
        string[i++] = (num % 10) + '0';
        num /= 10;
    }
    if(isneg){
        string[i++] = '-';
    }
    string[i] = '\0';

    uint8_t j = 0;
    while(j < i){
        char aux = string[j];
        toReturn[j] = string[i];
        i--;
        j++;
    }
}