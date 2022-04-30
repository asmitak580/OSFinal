#include "debug.h"
#include "ide.h"
#include "ext2.h"
#include "elf.h"
#include "machine.h"
#include "libk.h"
#include "config.h"

void drawRect(int offset, char *VGA) {
    // offset is top left corner of rectangle
    if (offset >= 64000) {
        return;
    }
    uint32_t columnHeight = 10;
    uint32_t rowWidth = 40;
    drawHorizLine(offset, VGA, rowWidth);
    // check here
    drawVertLine(offset, VGA, columnHeight);
    // check here
    drawVertLine(offset + rowWidth, VGA, columnHeight);
    // check here
    drawHorizLine(offset+(320*(columnHeight)), VGA, rowWidth);

}


void drawHorizLine(int start, char *VGA, int length) {
    for(int i = start; i < start + length; i++) {
        VGA[i] = 0xc0; //choose color
    }
}


void drawVertLine(int start, char *VGA, int height) {
    for(int i = start; i < start+height-2; i+= 321) {
        VGA[i] = 0xc0; // color
    }

}