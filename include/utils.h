#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <iostream>
#include "./interfaces.h"


ipAddress* parseIpAddress(const char* str);

std::ostream& operator<<(std::ostream& stream, const ipAddress& addr);

constexpr unsigned int hash(const char* str) {
    unsigned int h = 0;
    while (*str) {
        h = h * 31 + *str++;
    }
    return h;
}
int64_t parsePort(const char* str);

App* parseCommandArgs(int argc, char*argv[]);

void testInput(const App* app);

#endif  // INCLUDE_UTILS_H_
