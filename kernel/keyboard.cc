#include "keyboard.h"
#include "idt.h"
#include "debug.h"
#include "machine.h"


void KEYBOARD::init(void) {
   IDT::interrupt(9, (uint32_t)_keyboardHandler);
   outb(0x64,0xAE);
   
}

unsigned char readCode(void) {
	return inb(0x60);
}

unsigned char codeToValue(unsigned char code) {
	return ascii[code];
}

extern "C" void keyboardHandler(uint32_t* things) {
   Debug::printf("hit keyboard handler\n");
   unsigned char code = readCode();
   if (code <= 0x83) {
      unsigned char value = codeToValue(code);
      // r is red
      // g is green
      // b is blue
      // p is pink
      // o is orange
      // y is yellow
      // v is violet
      if(value == 'r') {
         VGA_color = 0x28;
      } else if(value == 'g') {
         VGA_color = 0x02;
      } else if(value == 'b') {
         VGA_color = 0x01;
      } else if(value == 'p') {
         VGA_color = 0x12;
      } else if(value == 'o') {
         VGA_color = 0x2A;
      } else if(value == 'y') {
         VGA_color = 0x14;
      } else if(value == 'v') {
         VGA_color = 0x05;
      }
   }
}
