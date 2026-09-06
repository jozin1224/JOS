#ifndef _CXXKERNEL_LIB_H
#define _CXXKERNEL_LIB_H

#include "../Include/cstdint.h"
#include "../Lib/vga.hpp"
#include "../Drivers/keyboard.h"
#include "user.hpp"
#define BUFFER_SIZE 256
#define MAX_USERS 15
User user_database[MAX_USERS];
User rootUser;
int total_users = 0;
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