#include "keyboard.h"
#include "idt.h"
#include "debug.h"


void KEYBOARD::init(void) {
   IDT::interrupt(9, (uint32_t)_keyboardHandler);
    //outb(0x12,);
}

extern "C" void keyboardHandler() {
   Debug::printf("hit keyboard handler");
}