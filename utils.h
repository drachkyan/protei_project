#ifndef UTILS_H
#define UTILS_H

#include <iostream>


enum class Role{
    CLIENT,
    ADMIN
};

struct ipAddress {
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    unsigned char b4;
    ipAddress(const unsigned char b1, const unsigned char b2,
        const unsigned char b3, const unsigned char b4):
        b1(b1), b2(b2), b3(b3), b4(b4) {}
};

ipAddress* parseIpAddress(const char* str);

std::ostream& operator<<(std::ostream& stream, const ipAddress& addr);


struct App {
    unsigned short port;
    ipAddress* addr;
    const char* lib;
    Role role;
    long i;
    ~App(){
        delete addr;
    }
};

constexpr unsigned int hash(const char* str);

long parsePort(const char* str);

App* parseCommandArgs(int argc, char*argv[]);


#endif // UTILS_H