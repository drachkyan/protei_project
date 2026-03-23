#include <utility>
#include <iostream>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include "../../include/Network/AppSettings.h"
#include "../../include/Network/NetworkAddress.h"
#include "../../client/include/utils/utils.h"

std::ostream& operator<<(std::ostream& stream, const IpAddress& addr) {
    stream
    << static_cast<u_int16_t>(addr.b1) <<'.'
    << static_cast<u_int16_t>(addr.b2) << '.'
    << static_cast<u_int16_t>(addr.b3) << '.'
    << static_cast<u_int16_t>(addr.b4);

    return stream;
}


AppSettings::AppSettings(std::string port_, std::string addr_, std::string lib_, Role role_, std::string username_):
    port(std::move(port_)), addr(std::move(addr_)), lib(std::move(lib_)), role(role_), username(std::move(username_))
{
    initFuncMap();
    initRoleMap();
    netAddr = NetworkAddress(port, addr);
}

AppSettings::AppSettings() {
    initFuncMap();
    initRoleMap();
    port="";
    addr="";
    lib="";
    role=Role::CLIENT;
    username="";
}

void AppSettings::initFuncMap() {
    funcMap["-p"] = [this](const char* arg) { this->parsePort(arg); };
    funcMap["-i"] = [this](const char* arg) { this->parseIpAddress(arg); };
    funcMap["-u"] = [this](const char* arg) { this->parseUsername(arg); };
    funcMap["-L"] = [this](const char* arg) { this->parseLib(arg); };
    funcMap["-r"] = [this](const char* arg) { this->parseRole(arg); };
}

void AppSettings::initRoleMap() {
    roleMap["Client"] = Role::CLIENT;
    roleMap["Admin"] = Role::ADMIN;
}

bool AppSettings::verifySettings() const {
    if (addr.empty()) {
        return false;
    }
    if (port.empty()) {
        return false;
    }
    return true;

}

void AppSettings::setRole(Role role_) {
    this->role = role_;
}

void AppSettings::setAddr(std::string addr_) {
    this->addr = std::move(addr_);
}

void AppSettings::setPort(std::string port_) {
    this->port = std::move(port_);
}


void AppSettings::setLib(std::string lib_) {
    this->lib = std::move(lib_);
}

void AppSettings::parseIpAddress(const char *str) {
    this->addr = str;
}

void AppSettings::parsePort(const char *str) {
    this->port = str;
}

void AppSettings::dispatchCommand(const char *command, char *arg) {
    auto it = funcMap.find(command);
    if (it != funcMap.end()) {
        it->second(arg);
        return;
    }
    spdlog::info("Неизвестный флаг");
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
    if (!verifySettings()) {
        spdlog::info("Отсутствуют нужные параметры");
        return;
    }
    std::cout << "Библиотека "<< this->lib << std::endl;
    std::cout << "Имя " << this->username << std::endl;
    std::cout << "Сетевой адрес " << this->netAddr.value() << std::endl;
}

void AppSettings::setUsername(std::string username_) {
    this->username = std::move(username_);
}

void AppSettings::parseCommandArgs(const int argc, char *argv[]) {
    if (argc == 2 && hash(argv[1]) == hash("--help")) {
        std::cout << "Справка по работе клиентского приложения\n"
        << "-i : указание ip адреса\n"
        << "-p : указание порта\n"
        << "--help : справка\n\n"
        << "Работа в меню: \nadd_vector int 3 1 2 3\n"
        << "Создает вектор длинной 3 состоящий из чисел 1 2 3\n"
        << "send_vector : отпправляет вектор на сервер\n";
        return;
    }
    if (argc % 2 == 0 || argc < 2) {
        spdlog::info("Неверное число аргументов командной строки");
        return;
    }
    for (int j = 1; j < argc; j+=2) {
        const char* flag = argv[j];
        char* value = argv[j + 1];
        dispatchCommand(flag, value);
    }

    if (verifySettings()) {
        this->netAddr = NetworkAddress(this->port, this->addr);
        return;
    }
    spdlog::info("Соединение не может быть установлено - не задан айпи адрес или порт");
}
