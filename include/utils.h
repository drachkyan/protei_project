#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <iostream>
#include "./interfaces.h"


ipAddress* parseIpAddress(const char* str);

std::ostream& operator<<(std::ostream& stream, const ipAddress& addr);

constexpr unsigned int hash(const char* str);

int64_t parsePort(const char* str);

App* parseCommandArgs(int argc, char*argv[]);


#endif  // INCLUDE_UTILS_H_
