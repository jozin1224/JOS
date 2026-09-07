// Copyright (c) 2026 jozin1224

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64


#include "io.h"

static const char kbd_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,
 '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',    0,
  ' ',  0
};

static inline char get_pressed_key(void) {
    if ((inb(KBD_STATUS_PORT) & 1) == 0) {
        return 0;
    }
    
    unsigned char scancode = inb(KBD_DATA_PORT);
    
    if (scancode & 0x80) {
        return 0;
    }
    
    if (scancode < 128) {
        return kbd_map[scancode];
    }
    
    return 0;
}

#endif
