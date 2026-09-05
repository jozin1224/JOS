// Copyright (c) 2026 jozin1224


#include "../Lib/vga.hpp"
#include "../Drivers/keyboard.h"
#include "user.hpp"
#define BUFFER_SIZE 256
#define MAX_USERS 15
User user_database[MAX_USERS];
int total_users = 0;


void Reboot() {
    __asm__ __volatile__ (
        "outb %0, %1" 
        : 
        : "a"((unsigned char)0xFE), "d"((unsigned short)0x64)
    ); // Cool Comment
    __asm__ __volatile__ (
        "lidt (%0)\n\t"
        "int $3"
        :
        : "r"(0)
    );
}

bool create_user(unsigned short id, const char* name, const char* password, bool isAdmin) {
    if (total_users >= MAX_USERS) {
        return false;
    }
    user_database[total_users].init(id, name, password, isAdmin);
    total_users++;
    return true;
}
User rootUser;
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
    create_user(0, "root", "portal", true);
    create_user(1, "randomuser", "", false);   
    Vga::Clean();
    Vga::DrawText("[OK] Kernel Loaded!!!\n");
    LoginState login_state = GET_USERNAME;
    char user_buffer[32] = {0};
    char pass_buffer[32] = {0};
    int login_index = 0;
    User* logged_user = nullptr; 
    Vga::DrawText("\nUser: ");
    while (login_state != SUCCESS) 
    {
        const char Key = get_pressed_key();
        if (Key != 0) 
        {
            if (Key == '\n' || Key == '\r') 
            {
                if (login_state == GET_USERNAME) 
                {
                    user_buffer[login_index] = '\0';
                    login_state = GET_PASSWORD;
                    login_index = 0;
                    Vga::DrawText("\nPassword: ");
                } 
                else if (login_state == GET_PASSWORD) 
                {
                    pass_buffer[login_index] = '\0';
                    bool login_valido = false;
                    for (int u = 0; u < total_users; u++) 
                    {
                        bool user_ok = true;
                        const char* real_user = user_database[u].getUsername();
                        int i = 0;
                        while (real_user[i] != '\0' || user_buffer[i] != '\0') {
                            if (real_user[i] != user_buffer[i]) { user_ok = false; break; }
                            i++;
                        }
                        if (user_ok && user_database[u].verifyPassword(pass_buffer)) 
                        {
                            login_valido = true;
                            logged_user = &user_database[u]; 
                            break; 
                        }
                    }
                    if (login_valido) 
                    {
                        Vga::DrawTextEx("\n[OK] Sucess\n", 0x0A);
                        if (logged_user->checkAdmin()) {
                            Vga::DrawText("[i] ADMIN\n\n");
                        } else {
                            Vga::DrawText("[i] USER\n\n");
                        }
                        login_state = SUCCESS;
                    } 
                    else 
                    {
                        Vga::DrawTextEx("\n[ERRO] Incorrect\n", 0x0C);
                        for(int j = 0; j < 32; j++) { user_buffer[j] = 0; pass_buffer[j] = 0; }
                        login_state = GET_USERNAME;
                        login_index = 0;
                        Vga::DrawText("\nUser: ");
                    }
                }
            } 
            else if (Key == '\b') 
            {
                if (login_index > 0) {
                    login_index--;
                    if (login_state == GET_USERNAME) user_buffer[login_index] = 0;
                    else pass_buffer[login_index] = 0;
                    Vga::WriteChar('\b', 0x07);
                    Vga::WriteChar(' ', 0x07);
                    Vga::WriteChar('\b', 0x07);
                }
            } 
            else if (Key >= 32 && Key <= 126 && login_index < 30) 
            {
                if (login_state == GET_USERNAME) 
                {
                    user_buffer[login_index++] = Key;
                    Vga::WriteChar(Key, 0x07);
                } 
                else if (login_state == GET_PASSWORD) 
                {
                    pass_buffer[login_index++] = Key;
                    Vga::WriteChar('*', 0x07);
                }
            }
        }
    }
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
                    Vga::DrawText("\nCommands: help, clear, fastfetch");
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
                else if (compare_string(input_buffer, "reboot")) {
                    Vga::DrawText("[KERNEL] Reboot");
                    Reboot();
                    while(1);
                }  
                else if (compare_string(input_buffer, "fastfetch")) {
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
