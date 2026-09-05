#include "../Include/cstdint.h"
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
