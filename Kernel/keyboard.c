#include <time.h>
#include <exceptions.h>
#include "videoDriver.h"
#include "keyboard.h"

#define BUFFER_SIZE 1024

static char msg[BUFFER_SIZE];
static uint8_t currentIndex = 0;
static uint8_t returnIndex = 0;

char getMsg(){
    msg[returnIndex-1]=0;
    if(currentIndex == returnIndex || returnIndex==BUFFER_SIZE)
        return 0;
    else
        return msg[returnIndex++];
}

static char capslock = 0;
static char shift = 0;

//https://wiki.osdev.org/PS/2_Keyboard
static const char keyboard_map [128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n' /*enter*/,
        0 /*caps lock*/, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0 /*left shift*/, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0 /*right shift*/,
        0 /*left ctrl*/, 0, ' ', 0, 0, 0,   0,   0,   0,   0,   0,   0,   0, 0,  0, 0, 0,0, 0, 0,0,  0,0,  0,0,0,0,0,0,0,0 ,0
};
static const char shift_map[] = {
        0, '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P','{', '}', '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '"', '|',
        0, '|','Z', 'X', 'C', 'V', 'B', 'N', 'M','<', '>', '?', 0,
        0, 0, ' ',  0, 0, 0,   0,   0,   0,   0,   0,   0,   0, 0,  0, 0, 0,0, 0, 0,0,  0,0,  0,0,0,0,0,0,0,0 ,0
};

/*
Scan code	Key	Scan code	Key	Scan code	Key	Scan code	Key
0x01	escape pressed	0x02	1 pressed	0x03	2 pressed
0x04	3 pressed	0x05	4 pressed	0x06	5 pressed	0x07	6 pressed
0x08	7 pressed	0x09	8 pressed	0x0A	9 pressed	0x0B	0 (zero) pressed
0x0C	- pressed	0x0D	= pressed	0x0E	backspace pressed	0x0F	tab pressed
0x10	Q pressed	0x11	W pressed	0x12	E pressed	0x13	R pressed
0x14	T pressed	0x15	Y pressed	0x16	U pressed	0x17	I pressed
0x18	O pressed	0x19	P pressed	0x1A	[ pressed	0x1B	] pressed
0x1C	enter pressed	0x1D	left control pressed	0x1E	A pressed	0x1F	S pressed
0x20	D pressed	0x21	F pressed	0x22	G pressed	0x23	H pressed
0x24	J pressed	0x25	K pressed	0x26	L pressed	0x27	 ; pressed
0x28	' (single quote) pressed	0x29	` (back tick) pressed	0x2A	left shift pressed	0x2B	\ pressed
0x2C	Z pressed	0x2D	X pressed	0x2E	C pressed	0x2F	V pressed
0x30	B pressed	0x31	N pressed	0x32	M pressed	0x33	, pressed
0x34	. pressed	0x35	/ pressed	0x36	right shift pressed	0x37	(keypad) * pressed
0x38	left alt pressed	0x39	space pressed	0x3A	CapsLock pressed	0x3B	F1 pressed
0x3C	F2 pressed	0x3D	F3 pressed	0x3E	F4 pressed	0x3F	F5 pressed
0x40	F6 pressed	0x41	F7 pressed	0x42	F8 pressed	0x43	F9 pressed
0x44	F10 pressed	0x45	NumberLock pressed	0x46	ScrollLock pressed	0x47	(keypad) 7 pressed
0x48	(keypad) 8 pressed	0x49	(keypad) 9 pressed	0x4A	(keypad) - pressed	0x4B	(keypad) 4 pressed
0x4C	(keypad) 5 pressed	0x4D	(keypad) 6 pressed	0x4E	(keypad) + pressed	0x4F	(keypad) 1 pressed
0x50	(keypad) 2 pressed	0x51	(keypad) 3 pressed	0x52	(keypad) 0 pressed	0x53	(keypad) . pressed
0x57	F11 pressed
0x58	F12 pressed
0x81	escape released	0x82	1 released	0x83	2 released
0x84	3 released	0x85	4 released	0x86	5 released	0x87	6 released
0x88	7 released	0x89	8 released	0x8A	9 released	0x8B	0 (zero) released
0x8C	- released	0x8D	= released	0x8E	backspace released	0x8F	tab released
0x90	Q released	0x91	W released	0x92	E released	0x93	R released
0x94	T released	0x95	Y released	0x96	U released	0x97	I released
0x98	O released	0x99	P released	0x9A	[ released	0x9B	] released
0x9C	enter released	0x9D	left control released	0x9E	A released	0x9F	S released
0xA0	D released	0xA1	F released	0xA2	G released	0xA3	H released
0xA4	J released	0xA5	K released	0xA6	L released	0xA7	 ; released
0xA8	' (single quote) released	0xA9	` (back tick) released	0xAA	left shift released	0xAB	\ released
0xAC	Z released	0xAD	X released	0xAE	C released	0xAF	V released
0xB0	B released	0xB1	N released	0xB2	M released	0xB3	, released
0xB4	. released	0xB5	/ released	0xB6	right shift released	0xB7	(keypad) * released
0xB8	left alt released	0xB9	space released	0xBA	CapsLock released	0xBB	F1 released
0xBC	F2 released	0xBD	F3 released	0xBE	F4 released	0xBF	F5 released
0xC0	F6 released	0xC1	F7 released	0xC2	F8 released	0xC3	F9 released
0xC4	F10 released	0xC5	NumberLock released	0xC6	ScrollLock released	0xC7	(keypad) 7 released
0xC8	(keypad) 8 released	0xC9	(keypad) 9 released	0xCA	(keypad) - released	0xCB	(keypad) 4 released
0xCC	(keypad) 5 released	0xCD	(keypad) 6 released	0xCE	(keypad) + released	0xCF	(keypad) 1 released
0xD0	(keypad) 2 released	0xD1	(keypad) 3 released	0xD2	(keypad) 0 released	0xD3	(keypad) . released
0xD7	F11 released
0xD8	F12 released
0xE0, 0x10	(multimedia) previous track pressed
0xE0, 0x19	(multimedia) next track pressed
0xE0, 0x1C	(keypad) enter pressed	0xE0, 0x1D	right control pressed
0xE0, 0x20	(multimedia) mute pressed	0xE0, 0x21	(multimedia) calculator pressed	0xE0, 0x22	(multimedia) play pressed
0xE0, 0x24	(multimedia) stop pressed
0xE0, 0x2E	(multimedia) volume down pressed
0xE0, 0x30	(multimedia) volume up pressed			0xE0, 0x32	(multimedia) WWW home pressed
0xE0, 0x35	(keypad) / pressed
0xE0, 0x38	right alt (or altGr) pressed
0xE0, 0x47	home pressed
0xE0, 0x48	cursor up pressed	0xE0, 0x49	page up pressed			0xE0, 0x4B	cursor left pressed
0xE0, 0x4D	cursor right pressed			0xE0, 0x4F	end pressed
0xE0, 0x50	cursor down pressed	0xE0, 0x51	page down pressed	0xE0, 0x52	insert pressed	0xE0, 0x53	delete pressed
0xE0, 0x5B	left GUI pressed
0xE0, 0x5C	right GUI pressed	0xE0, 0x5D	"apps" pressed	0xE0, 0x5E	(ACPI) power pressed	0xE0, 0x5F	(ACPI) sleep pressed
0xE0, 0x63	(ACPI) wake pressed
0xE0, 0x65	(multimedia) WWW search pressed	0xE0, 0x66	(multimedia) WWW favorites pressed	0xE0, 0x67	(multimedia) WWW refresh pressed
0xE0, 0x68	(multimedia) WWW stop pressed	0xE0, 0x69	(multimedia) WWW forward pressed	0xE0, 0x6A	(multimedia) WWW back pressed	0xE0, 0x6B	(multimedia) my computer pressed
0xE0, 0x6C	(multimedia) email pressed	0xE0, 0x6D	(multimedia) media select pressed
0xE0, 0x90	(multimedia) previous track released
0xE0, 0x99	(multimedia) next track released
0xE0, 0x9C	(keypad) enter released	0xE0, 0x9D	right control released
0xE0, 0xA0	(multimedia) mute released	0xE0, 0xA1	(multimedia) calculator released	0xE0, 0xA2	(multimedia) play released
0xE0, 0xA4	(multimedia) stop released
0xE0, 0xAE	(multimedia) volume down released
0xE0, 0xB0	(multimedia) volume up released			0xE0, 0xB2	(multimedia) WWW home released
0xE0, 0xB5	(keypad) / released
0xE0, 0xB8	right alt (or altGr) released
0xE0, 0xC7	home released
0xE0, 0xC8	cursor up released	0xE0, 0xC9	page up released			0xE0, 0xCB	cursor left released
0xE0, 0xCD	cursor right released			0xE0, 0xCF	end released
0xE0, 0xD0	cursor down released	0xE0, 0xD1	page down released	0xE0, 0xD2	insert released	0xE0, 0xD3	delete released
0xE0, 0xDB	left GUI released
0xE0, 0xDC	right GUI released	0xE0, 0xDD	"apps" released	0xE0, 0xDE	(ACPI) power released	0xE0, 0xDF	(ACPI) sleep released
0xE0, 0xE3	(ACPI) wake released
0xE0, 0xE5	(multimedia) WWW search released	0xE0, 0xE6	(multimedia) WWW favorites released	0xE0, 0xE7	(multimedia) WWW refresh released
0xE0, 0xE8	(multimedia) WWW stop released	0xE0, 0xE9	(multimedia) WWW forward released	0xE0, 0xEA	(multimedia) WWW back released	0xE0, 0xEB	(multimedia) my computer released
0xE0, 0xEC	(multimedia) email released	0xE0, 0xED	(multimedia) media select released
0xE0, 0x2A, 0xE0, 0x37	print screen pressed
0xE0, 0xB7, 0xE0, 0xAA	print screen released
0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5	pause pressed
*/

void key_handler(){
    if(currentIndex==BUFFER_SIZE) {return;}
    uint64_t key = getInput();
    char c = 0;
    switch(key) {
    case 0x1C:
        c = '\n';
        newLine();
        break;
    case 0x0E:
        c = '\b';
        break;
    case 0x1D:
        //capturar registros
        _printRegisters();
        break;
    case 0x3A:
        if(capslock==1)
            capslock=0;
        else
            capslock=1;
        break;
    //por prueba y error: release de shift izquierdo es -56, del derecho es -4A.
    case -0x56: case -0x4A:
        shift=0;
        break;
    case 0x2A: case 0x36:
        shift=1;
        break;
    default:
        c = keyboard_map[key];
        if(c!=0){
            if((c >= 'a' && c <= 'z') && ((shift && !capslock) || (!shift && capslock))) {
                c = shift_map[key];
            } else if (!(c >= 'a' && c <= 'z') && shift){
                c = shift_map[key];
            }
        }
    }
    //por afuera para guardar '\n' y '\b' tambien
    if(c!=0) {
        msg[currentIndex++] = c;
        msg[currentIndex] = 0;
    }
}