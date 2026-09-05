#include "../Lib/vga.hpp"
#include "../Drivers/keyboard.h"
extern "C" void kernel_main() {
    char* video_memory = (char*)0xB8000;
    Vga::Clean();
    Vga::DrawText("/Dev/sda> ");
    
    while(true)
    {
        const char Key = get_pressed_key();
        if (Key != 0)
        {

            Vga::WriteChar(Key, 0x07);
            if (Key == '\n')
            {
                Vga::DrawText("/Dev/sda> ");
            }
        }
    }
}
