#include "debug.h"
#include "libk.h"
#include "config.h"
#include "keyboard.h"
#include "graphics.h"


void drawHorizLine(uint32_t start, char *VGA, uint32_t length, int VGA_color) {
    // i mod 320 is 1
    for(uint32_t i = start; i < start + length && i < 64000; i++) {
        VGA[i] = VGA_color; //choose color
    }
}


void drawVertLine(uint32_t start, char *VGA, uint32_t height, int VGA_color) {
    
    for(uint32_t i = start; i <= (start+320*height) && i < 64000; i+= 320) {
        VGA[i] = VGA_color; // color
    }

}

void drawRect(uint32_t offset, char *VGA, int VGA_color) {
    // offset is top left corner of rectangle
    if (offset >= 64000) {
        return;
    }
    uint32_t columnHeight = 10;
    uint32_t rowWidth = 40;
    drawHorizLine(offset, VGA, rowWidth, VGA_color);

    drawVertLine(offset, VGA, columnHeight, VGA_color);
    drawVertLine(offset + rowWidth, VGA, columnHeight, VGA_color);
    drawHorizLine(offset+(320*(columnHeight)), VGA, rowWidth, VGA_color);

}