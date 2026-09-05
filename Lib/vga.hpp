// Copyright (c) 2026 jozin1224
// VGA.hpp
#include "../include/stdint.h"
int Lines = -1;
static int cursor_row = 0;
static int cursor_col = 0;
char* video_memory = (char*)0xB8000;

namespace Vga
{
    inline void WriteChar(char c, uint8_t color) {
        if (c == '\n') {
            cursor_col = 0;
            cursor_row++;
            Lines++;
        } else {
            int index = (cursor_row * 80 + cursor_col) * 2;
            video_memory[index] = c;
            video_memory[index + 1] = color;
            cursor_col++;
        }

        if (cursor_col >= 80) {
            cursor_col = 0;
            cursor_row++;
        }

        if (cursor_row >= 25) {
            cursor_row = 0;
        }
    }
    
    void DrawText(const char* message)
    {
        if (message == nullptr)
        {
            return;
        }
        for (int i = 0; message[i] != '\0'; ++i) {
            WriteChar(message[i], 0x07);
        }
    }
    
    void DrawTextEx(const char* message, uint8_t Color)
    {
        if (message == nullptr)
        {
            return;
        }
        for (int i = 0; message[i] != '\0'; ++i) {
            WriteChar(message[i], Color);
        }
    }
    
    void Clean() {
        for (int i = 0; i < 80 * 25 * 2; i += 2) {
            video_memory[i] = ' ';
            video_memory[i + 1] = 0x07;
        }
        cursor_row = 0;
        cursor_col = 0;
    }

}
