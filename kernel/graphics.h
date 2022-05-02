#ifndef _GRAPHICS_H
#define _GRAPHICS_H


extern void drawHorizLine(uint32_t start, char *VGA, uint32_t length, int VGA_color);
extern void drawVertLine(uint32_t start, char *VGA, uint32_t height, int VGA_color);
extern void drawDiagLeft(uint32_t start, char *VGA, uint32_t height, int VGA_color);
extern void drawDiagRight(uint32_t start, char *VGA, uint32_t height, int VGA_color);
extern void drawRect(uint32_t offset, char *VGA, int VGA_color);
extern void drawTriangle(uint32_t offset, char *VGA, int VGA_color);
extern void drawTrap(uint32_t offset, char *VGA, int VGA_color);
#endif