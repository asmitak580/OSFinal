#include "debug.h"
#include "libk.h"
#include "config.h"
#ifndef _KEYBOARD_H
#define _KEYBOARD_H
// ascii array from https://github.com/HasiniSama/pocketOS/tree/interupt_and_inputs/drivers
// r is red
// g is green
// b is blue
// p is pink
// o is orange
// y is yellow
// v is violet
// T IS TURQUOISE
extern uint32_t offset;
extern int VGA_color;
extern uint32_t rot[7];
extern unsigned char ascii[256];
extern void clear(uint32_t offset, char* VGA, uint32_t len, unsigned char value);
extern void rotate(unsigned char value, char* VGA);
extern void printBlock(char unsigned value, char* VGA);
class KEYBOARD {
public:
    static void init(void);
};

#endif