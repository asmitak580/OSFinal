#include "keyboard.h"
#include "idt.h"
#include "debug.h"
#include "machine.h"

int VGA_color = 0x0c;
unsigned char ascii[256] = {
    0x0, 0x0, '1', '2', '3', '4', '5', '6',		// 0 - 7
    '7', '8', '9', '0', '-', '=', 0x0, 0x0,		// 8 - 15
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',		// 16 - 23
    'o', 'p', '[', ']', '\n', 0x0, 'a', 's',	// 24 - 31
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',		// 32 - 39
    '\'', '`', 0x0, '\\', 'z', 'x', 'c', 'v',	// 40 - 47
    'b', 'n', 'm', ',', '.', '/', 0x0, '*',		// 48 - 55
    0x0, ' ', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,		// 56 - 63
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, '7',		// 64 - 71
    '8', '9', '-', '4', '5', '6', '+', '1',		// 72 - 79
    '2', '3', '0', '.'				// 80 - 83
};

void KEYBOARD::init(void) {
   IDT::interrupt(9, (uint32_t)_keyboardHandler);
   // write interrupt vector to 0x12?
   outb(0x21,0xFD);
   // write local apic id to 0x13?
   
}

unsigned char readCode(void) {
	return inb(0x60);
}

unsigned char codeToValue(unsigned char code) {
	return ascii[code];
}

extern "C" void keyboardHandler(uint32_t* things) {
   //read scan code
   
   char *VGA = (char*)0xA0000;
    // A0000000
    // uint16_t offset = 0;
    // uint32_t x = 0;
    // uint32_t y = 20;
    for (int i = 0; i < 3200; i++) {
        VGA[i] = 0x0f;
        // offset = x + y * 320;
        // VGA[offset] = 4;
        // y += 2;
    }
   // Debug::printf("hit keyboard handler\n");
   // char* nul = nullptr;
   // nul[0] = 'k';
   unsigned char code = readCode();
   outb(0x20,0x20); //ACK
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
