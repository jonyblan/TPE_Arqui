#include <time.h>
#include <naiveConsole.h>
#include <stdio.h>

#define CHANGEABLE_CHARS 21

#define FULL_ON_FLAG 0xFFFFFFFF
#define FULL_OFF_FLAG 0x00000000
#define SHIFT_FLAG 0x80000000
#define CONTROL_FLAG 0x40000000
#define ALT_FLAG 0x20000000
#define CAPS_LOCK_FLAG 0x10000000
#define NUM_LOCK_FLAG 0x08000000

static int cantFunctions = 1;

int8_t key_pressed();

static uint32_t flags = FULL_OFF_FLAG;

static char * validCalls[] = {
	"printDateTime",
	NULL
};

static char msg[256];
static int currentIndex = 0;

char getInput(){
	int8_t num = key_pressed();
	char aux = (char)num;
	char c = getCharacter(num, &flags);
	return c;
}

char changeControl(char c){
	return c;
}

char changeAlt(char c){
	return c;
}

char changeCapsLocked(char c){
	if(c >= 'a' && c <= 'z'){
		return (c - 'a' + 'A');
	}
	if(c >= 'A' && c <= 'Z'){
		return (c - 'A' + 'a');
	}
	return c;
}

char changeNumLocked(char c){
	return c;
}

char changeShift(char c){
	char noShiftTable[CHANGEABLE_CHARS] = {
		0x60,
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'0',
		'-',
		'=',
		'[',
		']',
		';',
		'\'',
		'\\',
		',',
		'.',
		'/'
	};

	char withShiftTable[CHANGEABLE_CHARS] = {
		'~',
		'!',
		'@',
		'#',
		'$',
		'%',
		'^',
		'&',
		'*',
		'(',
		')',
		'_',
		'+',
		'{',
		'}',
		':',
		'\"',
		'|',
		'<',
		'>',
		'?'
	};

	for (uint8_t i = 0; i < CHANGEABLE_CHARS; i++) {
		if (noShiftTable[i] == c) {
			return withShiftTable[i];
		}
	}

	c = changeCapsLocked(c);
	return c;
}

void commandSent(){
	msg[currentIndex] = '\0';
	ncMultipleLines(1);
	ncPrintln("Sending command...");
	for (int i = 0; validCalls[i] != NULL; i++) {
		if(compareStrings(validCalls[i], msg)){
			printDateTime();
			return ;
		}
   	}
	currentIndex = 0;
	ncPrintln("Command not found");
}

void key_handler() {
	char c = getInput();
	if(c == 0x0A){
		commandSent();
		ncPrintln("Que comando desea correr?");
		return ;
	}
	if(c=='\0'){
		return ;
	}
	if(flags & SHIFT_FLAG){
		c = changeShift(c);
	}
	if(flags & CONTROL_FLAG){
		c = changeControl(c);
	}
	if(flags & ALT_FLAG){
		c = changeAlt(c);
	}
	if(flags & CAPS_LOCK_FLAG){
		c = changeCapsLocked(c);
	}
	if(flags & NUM_LOCK_FLAG){
		c = changeNumLocked(c);
	}
	ncPrintChar(c);
	msg[currentIndex] = c;
	if(currentIndex < 254){
		currentIndex++;
	}
}