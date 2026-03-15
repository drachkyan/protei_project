#ifndef INCLUDE_NETWORK_CLIENT_H
#define INCLUDE_NETWORK_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"
#include "../../include/utils/utils.h"
#include "../../include/Model/VectorFactory.h"

using json = nlohmann::json;

class NetworkClient {
    int fd=-1;
    bool connectionFlag=false;
    static constexpr size_t BUF_SIZE = 1024;
    std::shared_ptr<NetworkAddress> address;

    int createConnection() {
        fd = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in addr{
            .sin_family = AF_INET,
            .sin_port   = htons(address->getPort()),
        };
        inet_pton(AF_INET, address->getIpAddress().c_str(), &addr.sin_addr);

        if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            return 1;
        }
        spdlog::info("Подключились");
        connectionFlag=true;
        return 0;
    }

    void sendJSON(const json& j) {
        if (fd == -1) {
            spdlog::info("Нет соединения");
            return;
        }
        spdlog::info("Начало отправки");
        std::string data = j.dump();
        uint32_t len = htonl(data.size());
        spdlog::info("Отправка");
        send(fd, &len, sizeof(len), 0);
        send(fd, data.c_str(), data.size(), 0);
    }

    json recvJson() {
        if (fd == -1) {
            spdlog::info("Нет соединения");
            return json{};
        }
        uint32_t len;
        recv(fd, &len, sizeof(len), 0);
        len = ntohl(len);

        std::string buf(len, '\0');
        recv(fd, buf.data(), len, 0);

        return json::parse(buf);
    }


public:
    void sendVector(AnyVector &vector) {
        json j{};
        anyVectorToJson(j, vector);
        if (j.empty()) {
            spdlog::info("Ошибка конвертации вектора в json\n");
            return;
        }
        spdlog::info("Вектор сконвертирован\n");
        sendJSON(j);
        std::cout << recvJson();
    }
    bool isConnected() const {return connectionFlag;}
    explicit NetworkClient(std::shared_ptr<NetworkAddress> address_)
                                : address(std::move(address_))
    {
        if (createConnection() != 0) {
            spdlog::info("Не удалось соединиться с сервером");
        }
    };
};


#endif  // INCLUDE_NETWORK_CLIENT_H