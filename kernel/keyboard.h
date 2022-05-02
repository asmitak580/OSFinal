#ifndef _KEYBOARD_H
#define _KEYBOARD_H
// r is red
// g is green
// b is blue
// p is pink
// o is orange
// y is yellow
// v is violet
extern int VGA_color;
extern unsigned char ascii[256];

class KEYBOARD {
public:
    static void init(void);
};

#endif