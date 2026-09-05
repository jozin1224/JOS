#include "../Lib/vga.hpp"

extern "C" void kernel_main() {
    char* video_memory = (char*)0xB8000;
    Vga::Clean();
    Vga::DrawText("Hello Moto");
    
    while(true);
}
