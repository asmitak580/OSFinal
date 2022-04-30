#include "keyboard.h"
#include "idt.h"
#include "debug.h"
#include "machine.h"


void KEYBOARD::init(void) {
   IDT::interrupt(9, (uint32_t)_keyboardHandler);
   outb(0x64,0xAB);
   
}

extern "C" void keyboardHandler(uint32_t* things) {
   Debug::printf("hit keyboard handler\n");
}