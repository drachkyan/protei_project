#include "../../include/Network/NetworkAddress.h"

#include <string>
#include "spdlog/spdlog.h"
#include <memory>
#include <iostream>

std::unique_ptr<IpAddress> NetworkAddress::parseIpAddress(const char* str) {
    uint8_t bytes[4];
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
        return nullptr;
    }
    return std::make_unique<IpAddress>(bytes[0], bytes[1], bytes[2], bytes[3]);
}

std::unique_ptr<IpAddress> NetworkAddress::parseHexIpAddress(const uint32_t hexAddr_) {
    uint8_t b0 = (hexAddr_ >> 24) & 0xFF;
    uint8_t b1 = (hexAddr_ >> 16) & 0xFF;
    uint8_t b2 = (hexAddr_>> 8)  & 0xFF;
    uint8_t b3 = (hexAddr_ >> 0)  & 0xFF;

    return std::make_unique<IpAddress>(b0, b1, b2, b3);
}

uint16_t NetworkAddress::parsePort(const char *str) {
    char* end = nullptr;

    const int64_t tempPort = strtol(str, &end, 10);

    if (*end != '\0') {
        spdlog::info("Ошибка при парсинге порта");
        return 0;
    }
    if (constexpr int64_t MAX_PORT = 65535;
        tempPort > MAX_PORT || tempPort <= 0) {
        spdlog::info("Порт должен быть в диапазоне 1 - 65535");
        return 0;
        }
    return tempPort;
}


NetworkAddress::NetworkAddress(const std::string& port_, const std::string& addr_) {
    this->addr = parseIpAddress(addr_.c_str());
    this->port = parsePort(port_.c_str());
    if (port == 0 || !addr ) {
        correctFlag = false;
    }

}

NetworkAddress::NetworkAddress(uint32_t addr_, uint16_t port_): port(port_) {
    // для адреса в 16сс
    this->addr = parseHexIpAddress(addr_);
}

std::ostream& operator<<(std::ostream& os, const NetworkAddress& na) {
    if (!na.addr || na.port == 0) {
        os << "invalid address";
        return os;
    }
    os << static_cast<int>(na.addr->b1) << "."
       << static_cast<int>(na.addr->b2) << "."
       << static_cast<int>(na.addr->b3) << "."
       << static_cast<int>(na.addr->b4) << ":"
       << na.port;
    return os;
}