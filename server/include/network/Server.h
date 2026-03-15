#ifndef INCLUDE_SERVER_H
#define INCLUDE_SERVER_H

#include <liburing.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>
#include "../../../client/include/Model/vector.hpp"
#include "spdlog/spdlog.h"
#include "../include/model/ServerHandlers.h"

using json = nlohmann::json;

enum class OpType {
    ACCEPT,
    RECV,
    SEND
};

struct OpContext {
    OpType type;
    int fd;
    std::unique_ptr<char[]> buf;
    size_t buf_size;

    OpContext(OpType type_, int fd_, size_t size)
        : type(type_), fd(fd_), buf(std::make_unique<char[]>(size)), buf_size(size) {}
};

using Handler = std::function<void(OpContext*, int)>;

struct ClientState {
    std::vector<char> buffer;
    uint32_t expected_len = 0;
    bool reading_len = true;
};

class Server {
    const int PORT;
    const int QUEUE_DEPTH;
    const size_t BUF_SIZE;
    io_uring ring{};
    int server_fd;
    sockaddr_in client_addr{};
    socklen_t addrlen = sizeof(client_addr);
    std::unordered_map<OpType, Handler> handlersMap;
    std::unordered_map<int, ClientState> clients;
    std::vector<OpContext*> pendingContexts;
    ServerHandlers handlers;
    bool running=true;

    void initHandlers();
    void addAccept();
    void addRecv(int client_fd);
    void addSend(int client_fd, const char* data, size_t len);

    void packageProcess(json& response, const char* buf);

    void handleAccept(OpContext* ctx, int res);
    void handleRecv(OpContext* ctx, int res);
    void handleSend(const OpContext* ctx, int res);
public:
    Server(int PORT_);
    ~Server();

    void run();

};

#endif  // INCLUDE_SERVER_H