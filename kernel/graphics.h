#ifndef _GRAPHICS_H
#define _GRAPHICS_H


extern void drawHorizLine(uint32_t start, char *VGA, uint32_t length, int VGA_color);
extern void drawVertLine(uint32_t start, char *VGA, uint32_t height, int VGA_color);
extern void drawDiagLeft(uint32_t start, char *VGA, uint32_t height, int VGA_color);
extern void drawDiagRight(uint32_t start, char *VGA, uint32_t height, int VGA_color);
extern void drawRect(uint32_t offset, char *VGA, int VGA_color);
extern void drawTriangle(uint32_t offset, char *VGA, int VGA_color);
extern void drawTrap(uint32_t offset, char *VGA, int VGA_color);
extern void drawHexa(uint32_t offset, char *VGA, int VGA_color);
extern void drawOcta(uint32_t offset, char *VGA, int VGA_color);
extern void IBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color);
extern void JBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color); 
extern void LBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color);
extern void OBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color); 
extern void SBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color);
extern void TBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color);
extern void ZBlock(uint32_t offset, char *VGA, uint32_t length, int VGA_color); 
extern void drawSquarePerimeter(uint32_t offset, char *VGA, uint32_t length, int VGA_color);   
extern void IBlockPerim(uint32_t offset, char *VGA, uint32_t length, int VGA_color);               
extern void JBlockPerim(uint32_t offset, char *VGA, uint32_t length, int VGA_color);                        
extern void LBlockPerim(uint32_t offset, char *VGA, uint32_t length, int VGA_color);                        
extern void OBlockPerim(uint32_t offset, char *VGA, uint32_t length, int VGA_color);                        
extern void SBlockPerim(uint32_t offset, char *VGA, uint32_t length, int VGA_color);                        
extern void TBlockPerim(uint32_t offset, char *VGA, uint32_t length, int VGA_color);                        
extern void ZBlockPerim(uint32_t offset, char *VGA, uint32_t length, int VGA_color);   
extern void clearIBlock(uint32_t offset, char *VGA, uint32_t length); 
extern void clearJBlock(uint32_t offset, char *VGA, uint32_t length);
extern void clearLBlock(uint32_t offset, char *VGA, uint32_t length);
extern void clearOBlock(uint32_t offset, char *VGA, uint32_t length);
extern void clearSBlock(uint32_t offset, char *VGA, uint32_t length);
extern void clearTBlock(uint32_t offset, char *VGA, uint32_t length);
extern void clearZBlock(uint32_t offset, char *VGA, uint32_t length); 
extern void bottomRow(char* VGA);               

#endif