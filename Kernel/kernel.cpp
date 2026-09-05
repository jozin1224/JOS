// Copyright (c) 2026 jozin1224


#include "../Lib/vga.hpp"
#include "../Drivers/keyboard.h"

#define BUFFER_SIZE 256
bool compare_string(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        i++;
    }
    return (str1[i] == str2[i]);
}

extern "C" void kernel_main() {
    Vga::Clean();
    Vga::DrawText("[OK] Kernel Loaded!!!\n");
    Vga::DrawTextEx("/Dev/sda> ", 0x0A);
    char input_buffer[BUFFER_SIZE];
    int buffer_index = 0;

    while(true)
    {
        const char Key = get_pressed_key();
        if (Key != 0)
        {
            if (Key == '\n')
            {
                input_buffer[buffer_index] = '\0';
                

                if (compare_string(input_buffer, "help")) {
                    Vga::DrawText("\nCommands: help, clear, fastfetch");
                    
                } 
                else if (compare_string(input_buffer, "clear")) {
                    Vga::Clean();
                    
                } 
                else if (compare_string(input_buffer, "fastfetch")) {

                    Vga::DrawText("\nJOS 1.0\nCopyright (c) 2026 jozin1224\n JOS is a template for others OS");
                    
                } 
                else if (buffer_index > 0) {
                    Vga::DrawText("\nUnknow Command");
                    
                }

                buffer_index = 0;
                Vga::DrawTextEx("\n/Dev/sda> ", 0x0A);
                

                if (Lines >= 25)
                {
                    Vga::Clean();
                    Vga::DrawTextEx("/Dev/sda> ", 0x0A);
                    Lines = 0;
                }
            }
            else if (Key == '\b') 
            {
                if (buffer_index > 0) {
                    buffer_index--;
                    Vga::WriteChar(Key, 0x07);
                }
            }
            else if (buffer_index < BUFFER_SIZE - 1) 
            {
                input_buffer[buffer_index++] = Key;
                Vga::WriteChar(Key, 0x07);
            }
        }
    }
}
