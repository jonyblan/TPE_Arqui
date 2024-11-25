#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

extern void outb(uint16_t arg1, uint16_t arg2);
extern uint16_t inb(uint16_t arg);

void mouse_handler();

#endif