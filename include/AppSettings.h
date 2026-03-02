//
// Created by vova on 01.03.2026.
//

#ifndef INCLUDE_APPSETTINGS_H
#define INCLUDE_APPSETTINGS_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <functional>

using func_type = std::function<void(char*)>;

struct ipAddress {
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    unsigned char b4;
    ipAddress(const unsigned char b1, const unsigned char b2,
        const unsigned char b3, const unsigned char b4):
        b1(b1), b2(b2), b3(b3), b4(b4) {}
};
enum class Role{
    CLIENT,
    ADMIN,
    NONE
};

class AppSettings {
    uint16_t port;
    ipAddress* addr;
    std::string lib;
    Role role;
    int64_t i;
    std::string username;
    std::unordered_map<std::string, func_type> funcMap;
    std::unordered_map<std::string, Role> roleMap;


    void initFuncMap();
    void initRoleMap();


    void dispatchCommand(const char* command, char* arg);

    void setRole(Role role_);
    void setAddr(ipAddress* addr_);
    void setPort(uint16_t port_);
    void setI(int64_t i_);
    void setLib(std::string lib_);

    void parsePort(const char* str);
    void parseIpAddress(const char* str);
    void parseI(const char* str);
    void parseUsername(const char* str);
    void parseRole(const char* str);
    void parseLib(const char* str);

public:
    ~AppSettings(){
        delete addr;
    }
    bool verifySettings();
    AppSettings(uint16_t port, ipAddress* addr, std::string lib, Role role, int64_t i, std::string username_);
    AppSettings();
    void printSettings() const;
    void setUsername(std::string username_);
    void parseCommandArgs(int argc, char*argv[]);
};


#endif  // INCLUDE_APPSETTINGS_H