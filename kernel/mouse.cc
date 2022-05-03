#include "mouse.h"
#include "idt.h"
#include "debug.h"
#include "machine.h"
#include "keyboard.h"


void MOUSE::init(void) {
    IDT::interrupt(10, (uint32_t)_mouseHandler);
    // Address the 2nd device
    outb(0x64, 0xd4);
    outb(0x60, 0xF4);
    while (inb(0x60) != 0xFA); /* Wait for ACK from mouse... */
    /* Tell mouse to enable interrupts (IRQ12) */
    outb(0x64, 0x20);

    unsigned char res = inb(0x60);
    res |= 1 << 1;

    outb(0x64, 0x60);
    outb(0x60, res);
}

extern "C" void mouseHandler(uint32_t* things) {
   // Debug::printf("hit mouse handler\n");
    // // uint32_t y = 20;
    inb(0x64);
    outb(0x20, 0x20);
   char *VGA = (char*)0xA0000;
    for (int i = 0; i < 3200; i++) {
        VGA[i] = VGA_color;
    }
    // char *VGA = (char*)0xA0000;
    // for (int i = 0; i < 3200; i++) {
    //     VGA[i] = VGA_color;
    //}
}