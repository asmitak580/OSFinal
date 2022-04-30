#include "debug.h"
#include "libk.h"
#include "config.h"
#include "keyboard.h"

void drawRect(int offset, char *VGA, int VGA_color) {
    // offset is top left corner of rectangle
    if (offset >= 64000) {
        return;
    }
    uint32_t columnHeight = 10;
    uint32_t rowWidth = 40;
    drawHorizLine(offset, VGA, rowWidth, VGA_color);

    drawVertLine(offset, VGA, columnHeight, VGA_color);
    drawVertLine(offset + rowWidth, VGA, columnHeight, VGA_color);
    drawHorizLine(offset+(320*(columnHeight))+1, VGA, rowWidth, VGA_color);

}


void drawHorizLine(int start, char *VGA, int length, int VGA_color) {
    // i mod 320 is 1
    for(int i = start; i < start + length && i < 64000; i++) {
        VGA[i] = VGA_color; //choose color
    }
}


void drawVertLine(int start, char *VGA, int height, int VGA_color) {
    for(int i = start; i < start+height-2 && i < 64000; i+= 321) {
        VGA[i] = VGA_color; // color
    }

}