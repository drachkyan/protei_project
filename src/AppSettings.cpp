#include <utility>
#include <iostream>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include "../include/AppSettings.h"


std::ostream& operator<<(std::ostream& stream, const ipAddress& addr) {
    stream
    << static_cast<u_int16_t>(addr.b1) <<'.'
    << static_cast<u_int16_t>(addr.b2) << '.'
    << static_cast<u_int16_t>(addr.b3) << '.'
    << static_cast<u_int16_t>(addr.b4);

    return stream;
}


AppSettings::AppSettings(uint16_t port_, ipAddress* addr_, std::string lib_, Role role_, int64_t i_,std::string username_):
port(port_), addr(addr_), lib(std::move(lib_)), role(role_), i(i_), username(std::move(username_)){
    initFuncMap();
    initRoleMap();
}

AppSettings::AppSettings() {
    initFuncMap();
    initRoleMap();
    port=0;
    addr=nullptr;
    lib="";
    role=Role::CLIENT;
    i=0;
    username="";
}

void AppSettings::initFuncMap() {
    funcMap["-p"] = [this](const char* arg) { this->parsePort(arg); };
    funcMap["-a"] = [this](const char* arg) { this->parseIpAddress(arg); };
    funcMap["-u"] = [this](const char* arg) { this->parseUsername(arg); };
    funcMap["-i"] = [this](const char* arg) { this->parseI(arg); };
    funcMap["-L"] = [this](const char* arg) { this->parseLib(arg); };
    funcMap["-r"] = [this](const char* arg) { this->parseRole(arg); };
}

void AppSettings::initRoleMap() {
    roleMap["Client"] = Role::CLIENT;
    roleMap["Admin"] = Role::ADMIN;
}

bool AppSettings::verifySettings() {
    if (role==Role::NONE) {
        spdlog::info("Не задана роль");
    }
    if (username.empty()) {
        spdlog::info("Не задано имя пользователя");
    }
    if (lib.empty()) {
        spdlog::info("Не задана библиотека");
    }
    if (i==-1) {
        spdlog::info("Не задан флаг -i");
    }
    if (!addr) {
        spdlog::info("Не задан ip address");
        return false;
    }
    if (port == 0) {
        spdlog::info("Не задан порт");
        return false;
    }
    return true;

}

void AppSettings::setRole(Role role_) {
    this->role = role_;
}

void AppSettings::setAddr(ipAddress *addr_) {
    this->addr = addr_;
}

void AppSettings::setPort(uint16_t port_) {
    this->port = port_;
}

void AppSettings::setI(int64_t i_) {
    this->i = i_;
}

void AppSettings::setLib(std::string lib_) {
    this->lib = std::move(lib_);
}

void AppSettings::dispatchCommand(const char *command, char *arg) {
    auto it = funcMap.find(command);
    if (it != funcMap.end()) {
        it->second(arg);
        return;
    }
    spdlog::info("Неизвестный флаг");
}

void AppSettings::parsePort(const char *str) {
    char* end = nullptr;
    const int64_t tempPort = strtol(str, &end, 10);
    if (*end != '\0') {
        spdlog::info("Ошибка при парсинге порта");
        return;
    }
    if (constexpr u_int64_t MAX_PORT = 65535;
        tempPort > MAX_PORT) {
        spdlog::info("Порт должен быть не больше 65535");
        return ;
        }
    this->setPort(tempPort);
}

void AppSettings::parseIpAddress(const char *str) {
    unsigned char bytes[4];
    char*end = nullptr;
    size_t counter = 0;

    for (int64_t i_ = std::strtol(str, &end, 10);
        str != end;
        i_ = std::strtol(str, &end, 10)) {
        if (constexpr int64_t MAX_BYTE = 255; i_ < 0 || i_ > MAX_BYTE) {
            break;
        }
        bytes[counter] = i_;

        str = end;
        if (*str == '.') {
            ++str;
        }
        counter++;

        if (counter == 4) {
            break;
        }
        }
    if (counter != 4 || *end != '\0') {
        spdlog::info("Ошибка во время парсинга айпи адреса");
        return;
    }
    this->setAddr( new ipAddress(bytes[0],
        bytes[1],
        bytes[2],
        bytes[3]));
}

void AppSettings::parseI(const char *str) {
    char* end = nullptr;
    this->setI(strtol(str, &end, 10));
}

void AppSettings::parseUsername(const char *str) {
    const std::string username_(str);
    if (username_.empty()) {
        spdlog::info("Пустое имя");
        return;
    }
    if (username_.size() > 32) {
        spdlog::info("Имя слишком длинное");
        return;
    }
    this->setUsername(username_);
}

void AppSettings::parseRole(const char *str) {
    auto it = roleMap.find(str);
    if (it != roleMap.end()) {
        this->setRole(it->second);
        return;
    }
    spdlog::info("Роль не найдена");
}

void AppSettings::parseLib(const char *str) {
    this->lib = str;
}

void AppSettings::printSettings() const {
    std::cout << "Библиотека "<< this->lib << std::endl;
    std::cout << "Имя " << this->username << std::endl;
    std::cout << "Айпи адрес " << *this->addr << std::endl;
    std::cout << "Порт " << this->port << std::endl;
    std::cout << "I " << this->i << std::endl;
}

void AppSettings::setUsername(std::string username_) {
    this->username = std::move(username_);
}

void AppSettings::parseCommandArgs(const int argc, char *argv[]) {
    if (argc % 2 == 0 || argc < 2) {
        spdlog::info("Неверное число аргументов командной строки");
        return;
    }
    for (int j = 1; j < argc; j+=2) {
        const char* flag = argv[j];
        char* value = argv[j + 1];
        dispatchCommand(flag, value);
    }
}
