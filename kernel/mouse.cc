#include "mouse.h"
#include "idt.h"
#include "debug.h"
#include "machine.h"
#include "keyboard.h"


void MOUSE::init(void) {
   IDT::interrupt(10, (uint32_t)_mouseHandler);
   outb(0x21,0xF4);
   
}

extern "C" void mouseHandler(uint32_t* things) {
   // Debug::printf("hit mouse handler\n");
    // // uint32_t y = 20;
    inb(0x60);
    outb(0x20, 0x20);
   char *VGA = (char*)0xA0000;
    for (int i = 0; i < 3200; i++) {
        VGA[i] = VGA_color;
    }
}