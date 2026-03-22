#ifndef INCLUDE_NETWORK_CLIENT_H
#define INCLUDE_NETWORK_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"
#include "../../include/utils/utils.h"
#include "../../../myvector/include/VectorFactory.h"

using json = nlohmann::json;

class NetworkClient {
    int fd=-1;
    std::thread ping_thread;
    std::atomic<bool> connectionFlag=false;
    static constexpr size_t BUF_SIZE = 1024;
    std::shared_ptr<NetworkAddress> address;


    void ping();

    int createConnection();



public:
    void sendJSON(const json& j) const;
    json recvJSON();

    bool isConnected() const {return connectionFlag;}
    explicit NetworkClient(std::shared_ptr<NetworkAddress> address_);
    ~NetworkClient();
};


#endif  // INCLUDE_NETWORK_CLIENT_H