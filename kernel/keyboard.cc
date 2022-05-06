#include "keyboard.h"
#include "idt.h"
#include "debug.h"
#include "machine.h"
#include "graphics.h"

uint32_t offset = 0;
unsigned char prev = 0;
int VGA_color = 0x0c;
uint32_t rot[7] = {0,0,0,0};
// translating keyboard scancodes into char values
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
   // initialize keyboard with interrupts
   // setting entry 9 of IDT to keyboardHandler
   IDT::interrupt(9, (uint32_t)_keyboardHandler);
   // unmask IRQ1 by sending interrupt mask to PIC1
   outb(0x21,0xFD);
}

unsigned char readCode(void) {
   // read scancode from IO data port 0x60
	return inb(0x60);
}

unsigned char codeToValue(unsigned char code) {
   // translate scancode to char value
	return ascii[code];
}

extern "C" void keyboardHandler(uint32_t* things) {
   // Debug::printf("hit keyboard handler\n");
   //read scan code
   unsigned char code = readCode();
   // acknowledge interrupt
   outb(0x20,0x20); 
   // initialize VGA array 
   char *VGA = (char*)0xA0000;
   // max code is 0x83
   if (code <= 0x83) {
      // find char value from scancode
      unsigned char value = codeToValue(code);
      // mapping keys to do many things :)
      if(value == 'r') { //red
         VGA_color = 0x28;
      } else if(value == 'g') { //green
         VGA_color = 0x30;
      } else if(value == 'b') { //blue
         VGA_color = 0x35;
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
         IBlockPerim(offset,VGA, 10, 0x03);
      } else if(value == '2') {
         prev = value;
         JBlock(offset, VGA, 10, 0x37);
         JBlockPerim(offset, VGA, 10, 0x01);
      } else if(value == '3') {
         prev = value;
         LBlock(offset, VGA, 10, 0x2A);    
         LBlockPerim(offset, VGA, 10, 0x06);        
      } else if(value == '4') {
         prev = value;
         OBlock(offset, VGA, 10, 0x2c);
         OBlockPerim(offset, VGA, 10, 0x74);
      } else if(value == '5') {
         prev = value;
         SBlock(offset, VGA, 10, 0x30);
         SBlockPerim(offset, VGA, 10, 0x02);
      } else if(value == '6') {
         prev = value;
         TBlock(offset, VGA, 10, 0x22);
         TBlockPerim(offset, VGA, 10, 0x6A);
      } else if(value == '7') {
         prev = value;
         ZBlock(offset, VGA, 10, 0x28);
         ZBlockPerim(offset, VGA, 10, 0x04);
      } else if(value == 'w') {
         if(prev != '0') {
            clear(offset, VGA, 10, prev);
         }
         offset -= 320*10;
         printBlock(prev, VGA);
      } else if(value == 's') {
         if(prev != '0') {
            clear(offset, VGA, 10, prev);

         }
         offset += 320*10;
         printBlock(prev, VGA);
      } else if(value == 'a') {
         if(prev != '0') {
            clear(offset, VGA, 10, prev);
         }
         offset -= 10;
         printBlock(prev, VGA);
      } else if(value == 'd') {
         if(prev != '0') {
            clear(offset, VGA, 10, prev);
         }
         offset += 10;
         printBlock(prev, VGA);
      } else if (value == '0') {
         clear(offset, VGA, 10, prev);
      } else if (value == '/') {
         offset = 0;
         prev = '0';
      } else if(value == ';') {
         rotate(prev, VGA);
         value = prev;
      }
   }
    //  for (int i = 0; i < 3200; i++) {
   //      VGA[i] = VGA_color;
   //  }
   // drawRect(1300+2920-30, VGA, VGA_color);
   // drawTriangle(1300+2920+30, VGA, VGA_color);
   // drawTrap(1300+2920-210, VGA, VGA_color);
   // drawOcta(1300+2920-160, VGA, VGA_color);
   // drawHexa(1300+2920-105, VGA, VGA_color);
  
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

void rotate(unsigned char value, char* VGA) {
   if (value == '1') {
      clearIBlock(offset, VGA, 10);
      rot[0]++;
      if(rot[0] >3) {
         rot[0] = 0;
      }
      IBlock(offset, VGA, 10, 0x34);
      IBlockPerim(offset,VGA, 10, 0x03);
   } else if(value == '2') {
      clearJBlock(offset, VGA, 10);
      rot[1]++;
      if(rot[1] >3) {
         rot[1] = 0;
      }
      JBlock(offset, VGA, 10, 0x37);
      JBlockPerim(offset, VGA, 10, 0x01);
   } else if(value == '3') {
      clearLBlock(offset, VGA, 10);  
      rot[2]++;      
      if(rot[2] >3) {
         rot[2] = 0;
      }
      LBlock(offset, VGA, 10, 0x2A);    
      LBlockPerim(offset, VGA, 10, 0x06);    
   } else if(value == '4') {
      clearOBlock(offset, VGA, 10);
      rot[3]++;
      if(rot[3] >3) {
         rot[3] = 0;
      }
      OBlock(offset, VGA, 10, 0x2c);
      OBlockPerim(offset, VGA, 10, 0x74);
   } else if(value == '5') {
      clearSBlock(offset, VGA, 10);
      rot[4]++;
      if(rot[4] >3) {
         rot[4] = 0;
      }
      SBlock(offset, VGA, 10, 0x30);
      SBlockPerim(offset, VGA, 10, 0x02);
   } else if(value == '6') {
      clearTBlock(offset, VGA, 10);
      rot[5]++;
      if(rot[5] >3) {
         rot[5] = 0;
      }
      TBlock(offset, VGA, 10, 0x22);
      TBlockPerim(offset, VGA, 10, 0x6A);
   } else if(value == '7') {
      clearZBlock(offset, VGA, 10);
      rot[6]++;
      if(rot[6] >3) {
         rot[6] = 0;
      }
      ZBlock(offset, VGA, 10, 0x28);
      ZBlockPerim(offset, VGA, 10, 0x04);
   }
}

void printBlock(char unsigned value, char* VGA) {
   if(value == '1') {
      prev = value;
      IBlock(offset, VGA, 10, 0x34);
      IBlockPerim(offset,VGA, 10, 0x03);
   } else if(value == '2') {
      prev = value;
      JBlock(offset, VGA, 10, 0x37);
      JBlockPerim(offset, VGA, 10, 0x01);
   } else if(value == '3') {
      prev = value;
      LBlock(offset, VGA, 10, 0x2A);    
      LBlockPerim(offset, VGA, 10, 0x06);        
   } else if(value == '4') {
      prev = value;
      OBlock(offset, VGA, 10, 0x2c);
      OBlockPerim(offset, VGA, 10, 0x74);
   } else if(value == '5') {
      prev = value;
      SBlock(offset, VGA, 10, 0x30);
      SBlockPerim(offset, VGA, 10, 0x02);
   } else if(value == '6') {
      prev = value;
      TBlock(offset, VGA, 10, 0x22);
      TBlockPerim(offset, VGA, 10, 0x6A);
   } else if(value == '7') {
      prev = value;
      ZBlock(offset, VGA, 10, 0x28);
      ZBlockPerim(offset, VGA, 10, 0x04);
   }
}