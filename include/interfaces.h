
#ifndef INCLUDE_INTERFACES_H_
#define INCLUDE_INTERFACES_H_

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

struct App {
    unsigned short port;
    ipAddress* addr;
    const char* lib;
    Role role;
    int64_t i;
    ~App(){
        delete addr;
    }
};

#endif  // INCLUDE_INTERFACES_H_
