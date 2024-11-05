#include <videoDriver.h>
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

static uint64_t cursorX = 0;
static uint64_t cursorY = 0;

struct coordinates{
    uint8_t row;
    uint8_t col;
};

/**
 * Salta una linea y vuelve al principio de linea
 */
void newLine(){
    cursorX = 0;
    cursorY++;

    //Opcion 2: relleno con vacios
    /*
     * for(int i=0; i<width; i++) { putChar(' '); }
     */
}

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
        case '¬': row = 5; col = 15; break;
    }
    struct coordinates coord;
    coord.row = row*CHARACTER_HEIGHT;
    coord.col = col*CHARACTER_WIDTH;
    return coord;
}

/*
//Podria no declarar las variables, pero la implementacion original las usaba
void codeToCoord(char code, uint8_t * rowPtr, uint8_t * colPtr){
    int row;
    int col;
    if(code >= '0' && code <= '9'){
        row = 1;
        col = code - '0';
    }
    if(code >= 'A' && code <= 'Z'){
        if(code<='O'){
            row = 2;
            col = code - 'A' + 1;
        } else {
            row = 3;
            col = code - 'P';
        }
    }
    if(code >= 'a' && code <= 'z'){
        if(code <= 'o'){
            row = 4;
            col = code - 'a' + 1;
        } else {
            row = 5;
            col = code - 'p';
        }
    }
    switch(code){
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
        case '¬': row = 5; col = 15; break;
    }
    *rowPtr = row*CHARACTER_HEIGHT;
    *colPtr = col*CHARACTER_WIDTH;
}


//Para cuando implementemos punteros
uint8_t * codeToPos(char code){
    uint8_t * row;
    uint8_t * col;
    codeToCoord(code, row, col);
    return &font_bitmap[*row][*col];
}
 */

/*
// Debería ser con punteros... pero no anda
void putCharRelativeFormat(uint8_t ** start, uint32_t hexColor){
    for (int i=0; i<CHARACTER_HEIGHT; i++){
        for(int j=0; j<CHARACTER_WIDTH; j++){
            putPixel(hexColor & (*(start+i+j) != 0 ? 0x00FFFFFF: 0x00000000), cursorX*CHARACTER_WIDTH + j, cursorY*CHARACTER_HEIGHT + i);
        }
    }
    cursorX++;
}
 */

/**
 * Imprime el caracter recibido de font_bitmap con formato y salta al siguiente espacio.
 * @param row fila del caracter en font_bitmap
 * @param col columna del caracter en font_bitmap
 * @param hexColor color en hexa 32b
 */
void putCharCoordf(struct coordinates coord, uint32_t hexColor){
    for (int i=0; i<CHARACTER_HEIGHT; i++){
        for(int j=0; j<CHARACTER_WIDTH; j++){
            putPixel(hexColor & (font_bitmap[i+coord.row][j+coord.col] != 0 ? 0x00FFFFFF: 0x00000000), cursorX*CHARACTER_WIDTH + j, cursorY*CHARACTER_HEIGHT + i);
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
    putCharCoordf(coord, 0x00FFFFFF);
}

/**
 * Imprime un caracter con formato y salta de linea
 * @param character caracter a imprimir
 * @param hexColor formato
 */
void putCharf(char character, uint32_t hexColor){
    struct coordinates coord = charToCoord(character);
    putCharCoordf(coord, hexColor);
}

void putChar(char character){
    putCharf(character, 0x00FFFFFF);
	newLine();
}

/**
 * Imprime un string con formato
 * @param string
 * @param hexColor
 */
void putsf(char * string, uint32_t hexColor){
    uint16_t width = VBE_mode_info->width;

    for(int i=0; string[i]!=0; i++){
        if(cursorX==width/CHARACTER_WIDTH){
            newLine();
        }
        putCharf(string[i], hexColor);
    }
    newLine();
}

/**
 * Imprime un string
 * @param string
 */
//Usar putsf(string, 0x00FFFFFF) no anda, pero con aux sí
void puts(char * string){
    char * aux = string;
    putsf(aux, 0x00FFFFFF);
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
            putPixel(hexColor & (font_bitmap[i+row][j+col] != 0 ? 0x00FFFFFF: 0x00000000), x+j, y+i);
        }
    }
}