#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include "idtLoader.h"
#include <time.h>
#include <videoDriver.h>

enum Colors { BLACK=0, BLUE, GREEN, LIGHT_BLUE, RED, MAGENTA,
        ORANGE, LIGHT_GREY, DARK_GREY, BRIGHT_BLUE, BRIGHT_GREEN,
        BRIGHT_LIGHT_BLUE, BRIGHT_RED, PINK, YELLOW, WHITE };

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern void load_idt();

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	//ncPrint("[x64BareBones]");
	//ncNewline();

	//ncPrint("CPU Vendor:");
	//ncPrint(cpuVendor(buffer));
	//ncNewline();

	//ncPrint("[Loading modules]");
	//ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	//ncPrint("[Done]");
	//ncNewline();
	//ncNewline();

	//ncPrint("[Initializing kernel's binary]");
	//ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);
/*
	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();*/
	return getStackBase();
}

int main()
{	
	ncPrint("[Kernel Main]");
	/*ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();
	
	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);*/
	ncMultipleLines(3);
	
	/*ncPrintln("Lo que hago yo:");
	ncPrintln("Materia:");
	ncPrintFormatln("Arquitectura de Computadoras", 226);
	
	ncMultipleLines(2);

	ncPrintln("Hora actual UTC:");
	printDateTime();
	
	ncMultipleLines(2);
	ncMultipleLines(2);
	int last = 0;
	ncPrintln("Load IDT:");
	load_idt();
	ncPrintln("Inf loop:");
	ncPrintln("Que comando desea correr?");
	while(1 == 1){

	}*/
    
    for(int i=0; i<8; i++){
        putPixel(0x0000FF00, i, 16);
    }

    putsf("Hooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooola,  si?",0x00FFFFFF);
    putsf("CHAU LUGO",0x00FF0000);
    puts(" !\"#$%&'()*+,-./");
    puts("0123456789:;<=>?");
    puts("@ABCDEFGHIJKLMNO");
    puts("PQRSTUVWXYZ[\\]^_");
    puts("`abcdefghijklmno");
    puts("pqrstuvwxyz{|}~¬");
    putChar('a');
    putCharf('b', 0x00FF00FF);

	ncPrint("[Finished]");
	return 0;
}