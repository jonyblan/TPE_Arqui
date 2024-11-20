#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <time.h>
#include "idtLoader.h"
#include "videoDriver.h"
#include "keyboard.h"
#include "syscalls.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

extern uint64_t resetMain();

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
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

int main()
{
	load_idt();
    fillSyscalls();

    /*
    char c = 0;
    while(1){
        c = getMsg();
        if(c != NULL) {
            if(c=='s'){
                break;
            } else {
                putChar(c);
            }
        }
    }
     */

    clear();
	((EntryPoint)sampleCodeModuleAddress)();
    clear();
	return 0;
}