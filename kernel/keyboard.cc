#include "keyboard.h"
#include "idt.h"
#include "debug.h"
#include "machine.h"
#include "graphics.h"

uint32_t offset = 0;
unsigned char prev = 0;
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
   
   // Debug::printf("hit keyboard handler\n");
   // char* nul = nullptr;
   // nul[0] = 'k';
   unsigned char code = readCode();
   outb(0x20,0x20); //ACK
   char *VGA = (char*)0xA0000;
   //  for (int i = 0; i < 3200; i++) {
   //      VGA[i] = VGA_color;
   //  }
   
   
   if (code <= 0x83) {
      unsigned char value = codeToValue(code);
      // r is red
      // g is green
      // b is blue
      // p is pink
      // o is orange
      // y is yellow
      // v is violet
      if(value == 'r') { //red
         // char *VGA = (char*)0xA0000;
         VGA_color = 0x28;
         // drawTriangle(4000, VGA, VGA_color);
      } else if(value == 'g') { //green
         VGA_color = 0x02;
      } else if(value == 'b') { //blue
         VGA_color = 0x01;
      } else if(value == 'p') { // pink
         VGA_color = 0x24;
      } else if(value == 'o') { //orange
         VGA_color = 0x2A;
      } else if(value == 'y') { //yellow
         VGA_color = 0x2c;
      } else if(value == 'v') { //violet
         VGA_color = 0x22;
      } else if(value == 't'){ //turquoise
         VGA_color = 0x34;
      } else if(value == '1') {
         prev = value;
         IBlock(offset, VGA, 10, 0x34);
         IBlockParam(offset,VGA, 10, 0x03);
      } else if(value == '2') {
         prev = value;
         JBlock(offset, VGA, 10, 0x37);
         JBlockParam(offset, VGA, 10, 0x01);
      } else if(value == '3') {
         prev = value;
         LBlock(offset, VGA, 10, 0x2A);    
         LBlockParam(offset, VGA, 10, 0x06);        
      } else if(value == '4') {
         prev = value;
         OBlock(offset, VGA, 10, 0x2c);
         OBlockParam(offset, VGA, 10, 0x74);
      } else if(value == '5') {
         prev = value;
         SBlock(offset, VGA, 10, 0x30);
         SBlockParam(offset, VGA, 10, 0x02);
      } else if(value == '6') {
         prev = value;
         TBlock(offset, VGA, 10, 0x22);
         TBlockParam(offset, VGA, 10, 0x6A);
      } else if(value == '7') {
         prev = value;
         ZBlock(offset, VGA, 10, 0x28);
         ZBlockParam(offset, VGA, 10, 0x04);
      } else if(value == 'w') {
         if(prev != '0') {
            clear(offset, VGA, 10, prev);
            prev = '0';
         }
         offset -= 320*10;
      } else if(value == 's') {
         if(prev != '0') {
            clear(offset, VGA, 10, prev);
            prev = '0';
         }
         offset += 320*10;
      } else if(value == 'a') {
         if(prev != '0') {
            clear(offset, VGA, 10, prev);
            prev = '0';
         }
         offset -= 40;
      } else if(value == 'd') {
         if(prev != '0') {
            clear(offset, VGA, 10, prev);
            prev = '0';
         }
         offset += 40;
      } else if (value == '0') {
         clear(offset, VGA, 10, prev);
      } else if (value == '/') {
         offset = 0;
         prev = '0';
      }
   }
  
}

void clear(uint32_t offset, char* VGA, uint32_t len, unsigned char value) {
   if (value == '1') {
         clearIBlock(offset, VGA, len);
      } else if(value == '2') {
         clearJBlock(offset, VGA, len);
      } else if(value == '3') {
         clearLBlock(offset, VGA, len);        
      } else if(value == '4') {
         clearOBlock(offset, VGA, len);
      } else if(value == '5') {
         clearSBlock(offset, VGA, len);
      } else if(value == '6') {
         clearTBlock(offset, VGA, len);
      } else if(value == '7') {
         clearZBlock(offset, VGA, len);
      }
}
