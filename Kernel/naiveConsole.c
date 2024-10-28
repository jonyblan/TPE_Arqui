#include <naiveConsole.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <videoDriver.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

// Recieves instruction which must follow RTC rules
// listed in https://stanislavs.org/helppc/cmos_ram.html
// and returns accordingly in BCD format
extern uint16_t ncTimeBcd(uint8_t instruction);

// WARNING: Can cause a forever loop is no key is pressed
// waits until a key is pressed anr returns the raw value
// which has to be decoded using getCharacter()
extern int8_t waitGetInputChar();

void ncPrintCharFormat(char character, uint8_t format){
	*currentVideo = character;
	currentVideo++;
	*currentVideo = format;
	currentVideo++;
}

// prints the message string with the format sent
// a brief explanation may be found in the wikipedia page
// https://en.wikipedia.org/wiki/VGA_text_mode
void ncPrintFormat(const char * string, uint8_t format){
	int i;

	for(i = 0; string[i] != 0; i++){
		ncPrintCharFormat(string[i], format);
	}
}

void ncPrintFormatln(const char * string, uint8_t format){
	ncPrintFormat(string, format);
	ncNewline();
}

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintln(const char * string){
	ncPrint(string);
	ncNewline();
}

void ncPrintChar(char character)
{
	*currentVideo = character;
	currentVideo += 2;
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncMultipleLines(int cantLines){
	for( ; cantLines > 0; cantLines--){
		ncNewline();
	}
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

// returns a static char array. It's length is already
// trimmed to not include leading zeros
char * numToString16(uint16_t num){
	static char ret[6];  // Only need 5 characters (up to 65535) + null terminator
	int index = 4;       // Start filling from the end
	ret[5] = '\0';       // Null-terminate the string

	if (num == 0) {		// Handle the special case where the number is 0
		ret[4] = '0';    
		return &ret[4];  // Return a pointer to the '0'
	}

	while (num > 0 && index >= 0) {
		ret[index] = (num % 10) + '0';  // Extract the last digit
		index--;
		num /= 10;
	}

	return &ret[index + 1];  // Return pointer to the start of the number
}

// returns a static char array. It's length is already
// trimmed to not include leading zeros
char * numToString32(uint32_t num){
	static char ret[11];  // Only need 11 characters (up to 4294967295) + null terminator
	int index = 9;       // Start filling from the end
	ret[10] = '\0';       // Null-terminate the string

	if (num == 0) {		// Handle the special case where the number is 0
		ret[9] = '0';    
		return &ret[9];  // Return a pointer to the '0'
	}

	while (num > 0 && index >= 0) {
		ret[index] = (num % 10) + '0';  // Extract the last digit
		index--;
		num /= 10;
	}

	return &ret[index + 1];  // Return pointer to the start of the number
}

char * numToString64(uint64_t num){
	static char ret[21];  // Only need 5 characters (up to 65535) + null terminator
	int index = 4;       // Start filling from the end
	ret[21] = '\0';       // Null-terminate the string

	if (num == 0) {		// Handle the special case where the number is 0
		ret[20] = '0';    
		return &ret[20];  // Return a pointer to the '0'
	}

	while (num > 0 && index >= 0) {
		ret[index] = (num % 10) + '0';  // Extract the last digit
		index--;
		num /= 10;
	}

	return &ret[index + 1];  // Return pointer to the start of the number
}

// TODO: Document this
int tickCounter(int num, int last){
	int sec = ticks_elapsed();
	if((sec % (18*num) == 0) && (sec != last)){
		char * secc = numToString16((uint16_t)(sec/18));
		ncPrintln(secc);
		last = sec;
	}
	return last;
}

// TODO: Migrate this function to asm
uint16_t bcd_to_decimal(uint16_t bcd) {
	uint16_t decimal = 0;
	uint16_t multiplier = 1;

	while (bcd > 0) {
		// Get the lower nibble (the rightmost 4 bits)
		int nibble = bcd & 0x0F;  

		// Add the nibble value to the decimal result
		decimal += nibble * multiplier;

		// Shift right by 4 bits to process the next nibble
		bcd >>= 4; 

		// Increase the multiplier by a factor of 10 for the next nibble
		multiplier *= 10;
	}

	return decimal;
}

// TODO: Re-use the 16-bit version (no idea how)
uint8_t bcd_to_decimal8(uint8_t bcd) {
	uint8_t decimal = 0;
	uint16_t multiplier = 1;

	while (bcd > 0) {
		// Get the lower nibble (the rightmost 4 bits)
		int nibble = bcd & 0x0F;  

		// Add the nibble value to the decimal result
		decimal += nibble * multiplier;

		// Shift right by 4 bits to process the next nibble
		bcd >>= 4; 

		// Increase the multiplier by a factor of 10 for the next nibble
		multiplier *= 10;
	}

	return decimal;
}

// calls the RTC with the sent code.
// returns a string with the number recieved
// https://stanislavs.org/helppc/cmos_ram.html
char * getRtc(uint8_t code){
	uint16_t timeBcd = ncTimeBcd(code);
	uint16_t timeInt = bcd_to_decimal(timeBcd);
	char * timeStr = numToString16(timeInt);
	return timeStr;
}

// TODO: Make a struct for Date and Time
void printDateTime(){
	puts(getRtc(7)); // Day
	puts("/");
	puts(getRtc(8)); // Month
	puts("/");
	puts(getRtc(9)); // year
	puts(" ");
	puts(getRtc(4)); // hours
	puts(":");
	puts(getRtc(2)); // minutes
	puts(":");
	puts(getRtc(0));// seconds
}

int compareStrings(const char *str1, const char *str2) {
	while (*str1 != '\0' && *str2 != '\0') {
		if (*str1 != *str2) {
			return 0;  // Strings differ
		}
		str1++;
		str2++;
	}

	// If both strings end at the same point, they are equal.
	// If one string ends earlier than the other, they are different.
	return (*str1 == '\0' && *str2 == '\0');
}


static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

#define FULL_ON_FLAG 0xFFFFFFFF
#define FULL_OFF_FLAG 0x00000000
#define SHIFT_FLAG 0x80000000
#define CONTROL_FLAG 0x40000000
#define ALT_FLAG 0x20000000
#define CAPS_LOCK_FLAG 0x10000000
#define NUM_LOCK_FLAG 0x08000000

void changeFlags(int index, uint32_t * flags, bool make){
	uint32_t flag;
	switch(index){
		case 0: case 1:
			flag = SHIFT_FLAG;
		break;
		case 2: case 3:
			flag = CONTROL_FLAG;
		break;
		case 4: case 5:
			flag = ALT_FLAG;
		break;
		case 6:
			flag = CAPS_LOCK_FLAG;
			if(*flags & CAPS_LOCK_FLAG){
				make = false;
			}
			else{	
				make = true;
			}
		break;
		case 7:
			flag = NUM_LOCK_FLAG;
			if(*flags & NUM_LOCK_FLAG){
				make = false;
			}
			else{	
				make = true;
			}
		break;
	}
	if(make){
		(*flags) |= flag;
	}
	else{
		(*flags) &= (flag ^ FULL_ON_FLAG);
	}
}

// table lookup for values listed in this webpage
// https://stanislavs.org/helppc/make_codes.html
// change cant_codes whenever adding or removing elements from the lookup table
// cant_codes_chars has the ammount of characters that can be written (alphanumericals, space, etc)
// cant_codes_total has the ammount of characters that can't be written (shift, enter, etc)
#define CANT_CODES_CHARS 48
#define CANT_CODES_TOTAL 56
char getCharacter(uint8_t makeCode, uint32_t * flags) {
   	// Lookup table for lowercase characters corresponding to make codes
   	char charLookup[CANT_CODES_CHARS] = {
		'1', // 0x02
		'2', // 0x03
		'3', // 0x04
		'4', // 0x05
		'5', // 0x06
		'6', // 0x07
		'7', // 0x08
		'8', // 0x09
		'9', // 0x0A
		'0', // 0x0B
		'a', // 0x1E
		'b', // 0x30
		'c', // 0x2E
		'd', // 0x20
		'e', // 0x12
		'f', // 0x21
		'g', // 0x22
		'h', // 0x23
		'i', // 0x17
		'j', // 0x24
		'k', // 0x25
		'l', // 0x26
		'm', // 0x32
		'n', // 0x31
		'o', // 0x18
		'p', // 0x19
		'q', // 0x10
		'r', // 0x13
		's', // 0x1F
		't', // 0x14
		'u', // 0x16
		'v', // 0x2F
		'w', // 0x11
		'x', // 0x2D
		'y', // 0x15
		'z', // 0x2C
		'-', // 0x0C
		'=', // 0x0D
		'[', // 0x1A
		']', // 0x1B
		';', // 0x27
		'\'', // 0x28
		'`', // 0x29
		'\\', // 0x2B
		',', // 0x33
		'.', // 0x34
		'/',  // 0x35
		0x0A // enter
	};

	// Lookup table for make codes
	uint8_t makeCodes[CANT_CODES_TOTAL] = {
		// this order has to be the same as in charLookup
		0x02, // '1'
		0x03, // '2'
		0x04, // '3'
		0x05, // '4'
		0x06, // '5'
		0x07, // '6'
		0x08, // '7'
		0x09, // '8'
		0x0A, // '9'
		0x0B, // '0'
		0x1E, // 'a'
		0x30, // 'b'
		0x2E, // 'c'
		0x20, // 'd'
		0x12, // 'e'
		0x21, // 'f'
		0x22, // 'g'
		0x23, // 'h'
		0x17, // 'i'
		0x24, // 'j'
		0x25, // 'k'
		0x26, // 'l'
		0x32, // 'm'
		0x31, // 'n'
		0x18, // 'o'
		0x19, // 'p'
		0x10, // 'q'
		0x13, // 'r'
		0x1F, // 's'
		0x14, // 't'
		0x16, // 'u'
		0x2F, // 'v'
		0x11, // 'w'
		0x2D, // 'x'
		0x15, // 'y'
		0x2C, // 'z'
		0x0C, // '-'
		0x0D, // '='
		0x1A, // '['
		0x1B, // ']'
		0x27, // ';'
		0x28, // '\''
		0x29, // '`'
		0x2B, // '\\'
		0x33, // ','
		0x34, // '.'
		0x35, // '/'
		0x1C, // enter

		0x2A, // make shift
		0xAA, // break shift
		0x1D, // make control
		0x9D, // break control
		0x38, // make alt
		0xB8, // break alt
		0x3A, // caps lock
		0x45, // num lock;
		
	};
	
	// searches through printable characters
	for (uint8_t i = 0; i < CANT_CODES_CHARS; i++) {
		if (makeCodes[i] == makeCode) {
			return charLookup[i];
		}
	}
	
	// searches through non-printable characters
	for(uint8_t i = CANT_CODES_CHARS; i < CANT_CODES_TOTAL; i++){
		if(makeCodes[i] == makeCode){
			bool make;
			if(!((i - CANT_CODES_CHARS) % 2)){
				make = true;
			}
			else{
				make = false;
			}
			changeFlags(i - CANT_CODES_CHARS, flags, make);
			i = CANT_CODES_TOTAL;
		}
	}

	return '\0';
}
