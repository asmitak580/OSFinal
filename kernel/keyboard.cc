#include "keyboard.h"
#include "idt.h"


void KEYBOARD::init(void) {
    IDT::interrupt(9, (uint32_t)_keyboardHandler);
    //outb(,);
}

extern "C" void keyboardHandler() {
    Debug::printf("hit keyboard handler");
}