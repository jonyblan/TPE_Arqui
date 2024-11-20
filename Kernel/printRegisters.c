#include <stdint.h>
#include "printRegisters.h"
#include "videoDriver.h"

void printRegisters(uint64_t regs[REGISTER_COUNT]){
    char buffer[10];
    puts("\nRegisters values:");
    print("rip: ");
    itoa(regs[0], buffer);
    printc(buffer, TURQUOISE);
    print("\trax: ");
    itoa(regs[0], buffer);
    printc(buffer, TURQUOISE);
    print("\trbx: ");
    itoa(regs[1], buffer);
    printc(buffer, TURQUOISE);
    print("\nrcx: ");
    itoa(regs[2], buffer);
    printc(buffer, TURQUOISE);
    print("\trdx: ");
    itoa(regs[3], buffer);
    printc(buffer, TURQUOISE);
    print("\trsi: ");
    itoa(regs[4], buffer);
    printc(buffer, TURQUOISE);
    print("\nrdi: ");
    itoa(regs[5], buffer);
    printc(buffer, TURQUOISE);
    print("\trbp: ");
    itoa(regs[6], buffer);
    printc(buffer, TURQUOISE);
    print("\tcs: ");
    itoa(regs[7], buffer);
    printc(buffer, TURQUOISE);
    print("\nr8: ");
    itoa(regs[8], buffer);
    printc(buffer, TURQUOISE);
    print("\tr9: ");
    itoa(regs[9], buffer);
    printc(buffer, TURQUOISE);
    print("\tr10: ");
    itoa(regs[10], buffer);
    printc(buffer, TURQUOISE);
    print("\nr11: ");
    itoa(regs[11], buffer);
    printc(buffer, TURQUOISE);
    print("\tr12: ");
    itoa(regs[12], buffer);
    printc(buffer, TURQUOISE);
    print("\tr13: ");
    itoa(regs[13], buffer);
    printc(buffer, TURQUOISE);
    print("\tr14: ");
    itoa(regs[15], buffer);
    printc(buffer, TURQUOISE);
    print("\nr15: ");
    itoa(regs[16], buffer);
    printc(buffer, TURQUOISE);
    print("\trflags: ");
    itoa(regs[17], buffer);
    printc(buffer, TURQUOISE);
    putChar('\n');
}