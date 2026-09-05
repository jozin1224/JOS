// Copyright (c) 2026 jozin1224
#ifndef _CXXKERNEL_USER_HPP
#define _CXXKERNEL_USER_HPP
#define USER_ROOT_ID 0x00
#include "../Include/cstdint.h"
enum LoginState { GET_USERNAME, GET_PASSWORD, SUCCESS };

class User {
private:
    uint16_t id;
    char username[32];
    uint32_t passwordHash;
    bool isAdmin;

    uint32_t hashPassword(const char* str) const {
        uint32_t hash = 5381;
        int c;
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash;
    }

    void safeStringCopy(char* dest, const char* src, size_t maxLength) {
        size_t i = 0;
        while (i < maxLength - 1 && src[i] != '\0') {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }

public:
    
    User() : id(0), passwordHash(0), isAdmin(false) {
        username[0] = '\0';
    }

    
    void init(uint16_t userId, const char* name, const char* plainPassword, bool adminStatus) {
        id = userId;
        isAdmin = adminStatus;
        safeStringCopy(username, name, sizeof(username));
        passwordHash = hashPassword(plainPassword); 
    }

    uint16_t getId() const { return id; }
    const char* getUsername() const { return username; }
    bool checkAdmin() const { return isAdmin; }

    bool verifyPassword(const char* inputPassword) const {
        return hashPassword(inputPassword) == passwordHash;
    }
};

#endif