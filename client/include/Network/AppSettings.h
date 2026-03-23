#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include "NetworkAddress.h"

using func_type = std::function<void(char*)>;


enum class Role{
    CLIENT,
    ADMIN,
    NONE
};

class AppSettings {
    std::string port;
    std::string addr;
    std::optional<NetworkAddress> netAddr = std::nullopt;
    std::string lib;
    Role role;
    std::string username;
    std::unordered_map<std::string, func_type> funcMap;
    std::unordered_map<std::string, Role> roleMap;


    void initFuncMap();
    void initRoleMap();


    void dispatchCommand(const char* command, char* arg);

    void setRole(Role role_);
    void setAddr(std::string addr_);
    void setPort(std::string port_);
    void setLib(std::string lib_);

    void parseIpAddress(const char *str);
    void parsePort(const char *str);
    void parseUsername(const char* str);
    void parseRole(const char* str);
    void parseLib(const char* str);

public:
    ~AppSettings() = default;
    bool verifySettings() const;
    AppSettings(std::string port_, std::string addr_, std::string lib_, Role role_, int64_t i_,std::string username_);
    AppSettings();
    void printSettings() const;
    void setUsername(std::string username_);
    void parseCommandArgs(int argc, char*argv[]);
    std::optional<NetworkAddress>& getNetworkAddress(){return netAddr;};
};

