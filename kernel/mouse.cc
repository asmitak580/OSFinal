#include "mouse.h"
#include "idt.h"
#include "debug.h"
#include "machine.h"


void MOUSE::init(void) {
   IDT::interrupt(10, (uint32_t)_mouseHandler);
   outb(0x64,0xAB);
   
}

extern "C" void mouseHandler(uint32_t* things) {
   Debug::printf("hit mouse handler\n");
}