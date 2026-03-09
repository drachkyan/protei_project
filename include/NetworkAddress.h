#ifndef INCLUDE_NETWORK_ADDRESS_H
#define INCLUDE_NETWORK_ADDRESS_H

#include <memory>
#include <string>
struct IpAddress {
    const uint8_t b1;
    const uint8_t b2;
    const uint8_t b3;
    const uint8_t b4;
    IpAddress(const uint8_t b1, const uint8_t b2,
        const uint8_t b3, const uint8_t b4):
        b1(b1), b2(b2), b3(b3), b4(b4) {}
};

class NetworkAddress {
    std::unique_ptr<IpAddress> addr;
    uint16_t port;

    static std::unique_ptr<IpAddress> parseIpAddress(const char* str);
    static std::unique_ptr<IpAddress> parseHexIpAddress(uint32_t hexAddr_);
    static uint16_t parsePort(const char* str);
public:
    friend std::ostream& operator<<(std::ostream& os, const NetworkAddress& na);

    NetworkAddress(const uint16_t port_, IpAddress* addr_):
        addr(addr_), port(port_) {}

    NetworkAddress(const std::string& port_, const std::string& addr_);

    NetworkAddress(uint32_t hexAddr_, uint16_t port_);
};


#endif  // INCLUDE_NETWORK_ADDRESS_H