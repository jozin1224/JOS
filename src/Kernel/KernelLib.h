#ifndef _CXXKERNEL_LIB_H
#define _CXXKERNEL_LIB_H

#include "../Include/cstdint.h"
#include "../Lib/vga.hpp"
#include "../Drivers/keyboard.h"
#include "../Lib/user.hpp"
#define BUFFER_SIZE 256
#define MAX_USERS 15
#define VOID void

char user_buffer[32] = {0};
char pass_buffer[32] = {0};
int login_index = 0;
User* logged_user = nullptr; 
LoginState login_state = GET_USERNAME;
char input_buffer[BUFFER_SIZE];
int buffer_index = 0;
User user_database[MAX_USERS];
User rootUser;
int total_users = 0;
VOID LoginScreen(void)
{
    logged_user = nullptr; 
    login_state = GET_USERNAME;
    buffer_index = 0;
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
                    bool valid_logon = false;
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
                            valid_logon = true;
                            logged_user = &user_database[u]; 
                            break; 
                        }
                    }
                    if (valid_logon) 
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
                    Vga::WriteChar(0x00, 0x07);
                }
            }
        }
    }
}


// CPU math
int Xor(int a, int b) { return a ^ b; }
int Or(int a, int b) { return a | b; }
int And(int a, int b) { return a & b; }
void int_to_string(int32_t num, char* buffer, uint8_t buffer_size) {
    int i = 0;
    bool is_negative = false;

    if (num == 0) {
        if (buffer_size > 1) {
            buffer[i++] = '0';
            buffer[i] = '\0';
        }
        return;
    }

    if (num < 0) {
        is_negative = true;
        num = -num;
    }

    while (num > 0 && i < (buffer_size - 1)) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    if (is_negative && i < (buffer_size - 1)) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

void Sleep(volatile DWORD count) {
    volatile DWORD i, j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < 1600; j++) {
            __asm("nop"); 
        }
    }
}

void Reboot() { // Reboot
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
static unsigned long int NextNumber = 1;

void srand(unsigned int Sement) {
    NextNumber = Sement;
}

int rand() { // I am totally random
    NextNumber = NextNumber * 1103515245 + 12345;
    return (unsigned int)(NextNumber / 65536) % 32768;
}
bool RandomBool()
{
    if ((rand() % 2) == 0)
    {
        return true;
    }
    return false;
}
bool create_user(unsigned short id, const char* name, const char* password, bool isAdmin) {
    if (total_users >= MAX_USERS) {
        return false;
    }
    user_database[total_users].init(id, name, password, isAdmin);
    total_users++;
    return true;
}

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

#endif