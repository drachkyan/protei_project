#pragma once

#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"
#include "../../include/utils/utils.h"

using json = nlohmann::json;

class NetworkClient {
    int fd=-1;
    std::mutex lock;
    std::thread ping_thread;
    std::atomic<bool> connectionFlag=false;
    static constexpr size_t BUF_SIZE = 1024;
    NetworkAddress address;


    void ping();

    int createConnection();



public:
    void sendJSON(const json& j) const;
    json recvJSON();
    void sendRecv(json& req, json& res);
    bool isConnected() const {return connectionFlag;}
    explicit NetworkClient(NetworkAddress& address_);
    ~NetworkClient();
};
