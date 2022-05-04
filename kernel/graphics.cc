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

void drawTriangle(uint32_t offset, char *VGA, int VGA_color) {
    if (offset >= 64000) {
        return;
    }
    uint32_t size = 40;
    drawHorizLine(offset, VGA, size, VGA_color);
    // drawDiagRight(offset + size, VGA, size, VGA_color); //trapezoid
    drawDiagRight(offset, VGA, size, VGA_color);
    // drawDiagLeft(offset, VGA, size, VGA_color);//trapezoid
    drawDiagLeft(offset + size, VGA, size, VGA_color);

}

void drawDiagLeft(uint32_t start, char *VGA, uint32_t height, int VGA_color) {
    for(uint32_t i = start; i <= (start+320*(height/2) + height/2) && i < 64000; i+= 319) {
        VGA[i] = VGA_color; // color
    }
}


void drawDiagRight(uint32_t start, char *VGA, uint32_t height, int VGA_color) {
    for(uint32_t i = start; i <= (start+320*(height/2) + height/2) && i < 64000; i+= 321) {
        VGA[i] = VGA_color; // color
    }
}

void drawTrap(uint32_t offset, char *VGA, int VGA_color) {
    if (offset >= 64000) {
        return;
    }
    uint32_t size = 25;
    drawHorizLine(offset, VGA, size, VGA_color);
    drawDiagRight(offset + size, VGA, size*2, VGA_color); //trapezoid
    drawDiagLeft(offset, VGA, size*2, VGA_color);//trapezoid
    drawHorizLine(offset+(320*size)-size, VGA, size*3, VGA_color);

}

void drawHexa(uint32_t offset, char *VGA, int VGA_color) {
    if (offset >= 64000) {
        return;
    }
    uint32_t size = 20;
    drawHorizLine(offset, VGA, size, VGA_color);
    drawDiagRight(offset + size, VGA, size*2, VGA_color); //trapezoid
    drawDiagLeft(offset, VGA, size*2, VGA_color);//trapezoid
    drawDiagRight(offset+(320*size)-size, VGA, size*2, VGA_color); //trapezoid
    drawDiagLeft(offset+(320*size)+2*size, VGA, size*2, VGA_color);//trapezoid
    drawHorizLine(offset+(320*size)*2, VGA, size, VGA_color);
}

void drawOcta(uint32_t offset, char *VGA, int VGA_color) {
    if (offset >= 64000) {
        return;
    }
    uint32_t size = 20;
    drawHorizLine(offset, VGA, size, VGA_color);
    drawDiagRight(offset + size, VGA, size, VGA_color); //trapezoid
    drawDiagLeft(offset, VGA, size, VGA_color);//trapezoid
    drawDiagRight(offset-size/2+(320*size*1.5), VGA, size, VGA_color); //trapezoid
    drawDiagLeft(offset+size*1.5+(320*size*1.5), VGA, size, VGA_color);//trapezoid
    drawHorizLine(offset+(320*size)*2, VGA, size, VGA_color);
    drawVertLine(offset+(320*size)/2-size/2, VGA, size,  VGA_color);
    drawVertLine(offset+(320*size)/2+size*1.5, VGA, size,  VGA_color);
}

void drawCircle(uint32_t x, uint32_t y, char *VGA, int VGA_color) {
     if ((x*320 + y*200)>= 64000) {
        return;
    }
   // uint32_t size = 20;
    
}

// squares!!!
void drawSquare(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    for(uint32_t i = 1; i <= length; i++) {
        drawHorizLine(offset+320*i,VGA, length, VGA_color);
    }
}
void drawSquareParameter(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawHorizLine(offset+320,VGA, length, VGA_color);
    drawHorizLine(offset+320*length,VGA, length, VGA_color);
    drawVertLine(offset+320, VGA, length-1,  VGA_color);
    drawVertLine(offset+320+length, VGA, length-1,  VGA_color);    
}
void SquareParamOverlap(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawHorizLine(offset+320,VGA, length, VGA_color);
    drawHorizLine(offset+320*(length+1),VGA, length, VGA_color);
    drawVertLine(offset+320, VGA, length-1,  VGA_color);
    drawVertLine(offset+320+length, VGA, length-1,  VGA_color);    
}

/////////////////////////////////////// Tetris shapes ////////////////////////////////////////////////////////////
// I BLOCK
void IBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquare(offset,VGA, length, VGA_color);
    drawSquare(offset+length,VGA, length, VGA_color);
    drawSquare(offset+length*2,VGA, length, VGA_color);
    drawSquare(offset+length*3,VGA, length, VGA_color);
}
void IBlockParam(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquareParameter(offset,VGA, length, VGA_color);
    drawSquareParameter(offset+length,VGA, length, VGA_color);
    drawSquareParameter(offset+length*2,VGA, length, VGA_color);
    drawSquareParameter(offset+length*3,VGA, length, VGA_color);
}



// J BLOCK
void JBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquare(offset+320,VGA, length, VGA_color);
    drawSquare(offset+320*length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length*2,VGA, length, VGA_color);
}
void JBlockParam(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquareParameter(offset+320,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length*2,VGA, length, VGA_color);
}

// L BLOCK
void LBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquare(offset+320+length*2,VGA, length, VGA_color);
    drawSquare(offset+320*length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length*2,VGA, length, VGA_color);
}
void LBlockParam(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquareParameter(offset+320+length*2,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length*2,VGA, length, VGA_color);
}

// O BLOCK
void OBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquare(offset+320,VGA, length, VGA_color);
    drawSquare(offset+320+length,VGA, length, VGA_color);
    drawSquare(offset+320*length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length,VGA, length, VGA_color);
}
void OBlockParam(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquareParameter(offset+320,VGA, length, VGA_color);
    drawSquareParameter(offset+320+length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length,VGA, length, VGA_color);
}

// S BLOCK
void SBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquare(offset+320+length,VGA, length, VGA_color);
    drawSquare(offset+320+length*2,VGA, length, VGA_color);
    drawSquare(offset+320*length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length,VGA, length, VGA_color);
}
void SBlockParam(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquareParameter(offset+320+length,VGA, length, VGA_color);
    drawSquareParameter(offset+320+length*2,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length,VGA, length, VGA_color);
}

// T BLOCK
void TBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquare(offset+320+length,VGA, length, VGA_color);
    drawSquare(offset+320*length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length*2,VGA, length, VGA_color);
}
void TBlockParam(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquareParameter(offset+320+length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length*2,VGA, length, VGA_color);
}

// Z BLOCK
void ZBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquare(offset+320,VGA, length, VGA_color);
    drawSquare(offset+320+length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length,VGA, length, VGA_color);
    drawSquare(offset+320*length+length*2,VGA, length, VGA_color);
}
void ZBlockParam(uint32_t offset, char *VGA, uint32_t length, int VGA_color) {
    drawSquareParameter(offset+320,VGA, length, VGA_color);
    drawSquareParameter(offset+320+length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length,VGA, length, VGA_color);
    drawSquareParameter(offset+320*length+length*2,VGA, length, VGA_color);
}

void clearIBlock(uint32_t offset, char *VGA, uint32_t length) {
    IBlock(offset, VGA, length, 0);
    IBlockParam(offset, VGA, length, 0);
}
void clearJBlock(uint32_t offset, char *VGA, uint32_t length){
    JBlock(offset, VGA, length, 0);
    JBlockParam(offset, VGA, length, 0);
}
void clearLBlock(uint32_t offset, char *VGA, uint32_t length){
    LBlock(offset, VGA, length, 0);
    LBlockParam(offset, VGA, length, 0);
}
void clearOBlock(uint32_t offset, char *VGA, uint32_t length){
    OBlock(offset, VGA, length, 0);
    OBlockParam(offset, VGA, length, 0);
}
void clearSBlock(uint32_t offset, char *VGA, uint32_t length){
    SBlock(offset, VGA, length, 0);
    SBlockParam(offset, VGA, length, 0);
}
void clearTBlock(uint32_t offset, char *VGA, uint32_t length){
    TBlock(offset, VGA, length, 0);
    TBlockParam(offset, VGA, length, 0);
}
void clearZBlock(uint32_t offset, char *VGA, uint32_t length){
    ZBlock(offset, VGA, length, 0);
    ZBlockParam(offset, VGA, length, 0);
}

void bottomRow(char* VGA) {
    //320*160 + 20
    IBlock(320*190 + 60, VGA, 10, 0x34);
    IBlockParam(320*190 + 60, VGA, 10, 0x03);
    JBlock(320*180 + 100, VGA, 10, 0x37);
    JBlockParam(320*180 + 100, VGA, 10, 0x01);
    OBlock(320*180 + 130, VGA, 10, 0x2c);
    OBlockParam(320*180 + 130, VGA, 10, 0x74);
    SBlock(320*170 + 110, VGA, 10, 0x30);
    SBlockParam(320*170 + 110, VGA, 10, 0x02);
    IBlock(320*190 + 150, VGA, 10, 0x34);
    IBlockParam(320*190 + 150, VGA, 10, 0x03);
    TBlock(320*180 + 190, VGA, 10, 0x22);
    TBlockParam(320*180 + 190, VGA, 10, 0x6a);
}

