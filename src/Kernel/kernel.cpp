// Copyright (c) 2026 jozin1224

#include "KernelLib.h"
#include "../Lib/time.h"
extern "C" void kernel_main() {
    create_user(0, "root", "portal", true);
    create_user(1, "randomuser", "", false);
    create_user(2, "adrian shephard", "123123", true); 
    Vga::Clean();
    BiosTime Time = get_bios_time();
    Vga::DrawText("Welcome to JOS 1.0\n");
    Vga::DrawText("[OK] Kernel Loaded!!!\n");
    LoginScreen();
    srand(Xor(And(Time.day, Time.year), Or(Time.month, Time.second)));
    Vga::DrawText("\n");
    Vga::DrawTextEx(logged_user->getUsername(), 0x0A);
    Vga::DrawTextEx("@JOS# ", 0x0A);
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
                    Vga::DrawText("\nCommands: help, clear, rootcommand, info, randoutb, logoff, date");
                }
                else if (compare_string(input_buffer, "rootcommand"))
                {
                    if (logged_user->checkAdmin())
                    {
                        Vga::DrawText("\nAdmin!!!");
                    }
                    else
                    {
                        Vga::DrawText("\nUser!!!");
                    }
                } 
                else if (compare_string(input_buffer, "clear")) {
                    Vga::Clean();
                }
                else if (compare_string(input_buffer, "date")) {
                    char buffer[12];
                    Vga::WriteChar('\n', 0x07);
                    if (Time.month < 10)
                    {
                        Vga::DrawText("0");
                    }
                    int_to_string(Time.month, buffer, 12);
                    Vga::DrawText(buffer);
                    Vga::WriteChar(':', 0x07);
                    if (Time.day < 10)
                    {
                        Vga::DrawText("0");
                    }
                    int_to_string(Time.day, buffer, 12);
                    Vga::DrawText(buffer);
                    Vga::WriteChar(':', 0x07);
                    int_to_string(Time.year, buffer, 12);
                    Vga::DrawText(buffer);
                }
                else if (compare_string(input_buffer, "randoutb")) {
                    int a = 0;
                    int b = 0;
                    while(1)
                    {
                        outb(a, b);
                        if (RandomBool()) // ?????
                        {
                            a++;
                        }
                        else
                        {
                            b++;
                        }
                        Sleep(500);
                    }
                }
                else if (compare_string(input_buffer, "reboot")) {
                    Vga::DrawText("[KERNEL] Reboot");
                    Reboot();
                    while(1);
                } 
                else if (compare_string(input_buffer, "logoff")) {
                    Vga::Clean();
                    LoginScreen();
                }   
                else if (compare_string(input_buffer, "info")) {
                    Vga::DrawText("\nJOS 1.0\nCopyright (c) 2026 jozin1224\n JOS is a template for others OS");
                } 
                else if (buffer_index > 0) {
                    Vga::DrawText("\nUnknow Command");
                }
                buffer_index = 0;
                Vga::DrawText("\n");
                Vga::DrawTextEx(logged_user->getUsername(), 0x0A);
                Vga::DrawTextEx("@JOS# ", 0x0A);
                if (Lines >= 25)
                {
                    Vga::Clean();
                    Vga::DrawText("\n");
                    Vga::DrawTextEx(logged_user->getUsername(), 0x0A);
                    Vga::DrawTextEx("@JOS# ", 0x0A);
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
