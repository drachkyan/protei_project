#include "../../include/network/Server.h"

#include "../../client/include/Model/VectorFactory.h"
#include "../../include/model/ServerHandlers.h"
#include "spdlog/spdlog.h"

using json = nlohmann::json;


Server::~Server() {
    for (auto* ctx : pendingContexts) {
        delete ctx;
    }
    io_uring_queue_exit(&ring);
    close(server_fd);
}

void Server::initHandlers() {
    handlersMap[OpType::ACCEPT] = [this](OpContext* ctx, int res) { handleAccept(ctx, res); };
    handlersMap[OpType::RECV]   = [this](OpContext* ctx, int res) { handleRecv(ctx, res); };
    handlersMap[OpType::SEND]   = [this](OpContext* ctx, int res) { handleSend(ctx, res); };
}

void Server::addAccept() {
    auto* ctx = new OpContext{OpType::ACCEPT, server_fd, BUF_SIZE};  // освобождается потом в деструкторе
    pendingContexts.push_back(ctx);
    auto* sqe = io_uring_get_sqe(&ring);
    io_uring_prep_accept(sqe, server_fd,
        reinterpret_cast<sockaddr*>(&client_addr), &addrlen, 0);
    io_uring_sqe_set_data(sqe, ctx);
}

void Server::addRecv(const int client_fd) {
    auto* ctx = new OpContext{OpType::RECV, client_fd, BUF_SIZE};  // освобождается потом в деструкторе
    pendingContexts.push_back(ctx);
    auto* sqe = io_uring_get_sqe(&ring);
    io_uring_prep_recv(sqe, client_fd, ctx->buf.get(), ctx->buf_size, 0);
    io_uring_sqe_set_data(sqe, ctx);
}

void Server::addSend(const int client_fd, const char *data, size_t len) {
    auto* ctx = new OpContext{OpType::SEND, client_fd, len};  // освобождается потом в деструкторе
    pendingContexts.push_back(ctx);
    std::copy_n(data, len, ctx->buf.get());
    auto* sqe = io_uring_get_sqe(&ring);
    io_uring_prep_send(sqe, client_fd, ctx->buf.get(), len, 0);
    io_uring_sqe_set_data(sqe, ctx);
}


void Server::packageProcess(json &response, const char *buf) {
    json req = json::parse(buf);
    handlers.HandleJSON(req,response);
}

void Server::handleAccept(OpContext *ctx, const int res) {
    if (res < 0) {
        spdlog::info("ошибка\n");
        return;
    }
    int client_fd = res;
    clients[client_fd] = ClientState{};
    spdlog::info("новый клиент fd={}\n" , client_fd);

    addAccept();
    addRecv(client_fd);
    io_uring_submit(&ring);
}

void Server::handleRecv(OpContext *ctx, const int res)  {
    if (res <= 0) {
        spdlog::info( "клиент fd={} отключился\n", ctx->fd);
        clients.erase(ctx->fd);
        close(ctx->fd);
        // running = false;  //  для теста на утечки
        return;
    }


    auto& state = clients[ctx->fd];

    state.buffer.insert(state.buffer.end(), ctx->buf.get(), ctx->buf.get() + res); //считал

    if (state.reading_len && state.buffer.size()>= sizeof(uint32_t)) {
        uint32_t len;
        std::memcpy(&len, state.buffer.data(), sizeof(uint32_t));
        state.expected_len = ntohl(len);
        state.buffer.erase(state.buffer.begin(), state.buffer.begin() + sizeof(uint32_t));
        state.reading_len = false;
    } //пакет с длинной

    if (!state.reading_len && state.buffer.size() >= state.expected_len) {
        std::string json_str(state.buffer.begin(), state.buffer.begin() + state.expected_len);
        state.buffer.erase(state.buffer.begin(), state.buffer.begin() + state.expected_len);
        state.reading_len = true;


        json response{};
        packageProcess(response, json_str.c_str());

        std::string data = response.dump();
        uint32_t len = htonl(data.size());

        addSend(ctx->fd, reinterpret_cast<const char*>(&len), sizeof(len));
        addSend(ctx->fd, data.c_str(), data.size());
        io_uring_submit(&ring);
        return;
    }

    addRecv(ctx->fd);
    io_uring_submit(&ring);
}

void Server::handleSend(const OpContext *ctx, const int res) {
    if (res < 0) {
        spdlog::info("ошибка отправки: {}\n", std::strerror(-res));
        return;
    }
    addRecv(ctx->fd);
    io_uring_submit(&ring);
}

Server::Server(int PORT_)  :
            PORT(PORT_), QUEUE_DEPTH(32), BUF_SIZE(1024)
{
    initHandlers();
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        spdlog::info("ошибка создания сокета");
    }

    const int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr = {.s_addr = INADDR_ANY}
    };

    if (bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        spdlog::info("ошибка - скорее всего порт занят");
        return;
    }
    if (listen(server_fd, 10) < 0) {
        spdlog::info("ошибка запуска сервера");
        return;
    }

    if (io_uring_queue_init(QUEUE_DEPTH, &ring, 0) < 0) {
        spdlog::info("ошибка создания io_uring");
        return;
    }

    spdlog::info("[сервер]: запущен на порту {}\n", PORT);
}

void Server::run() {
    addAccept();
    io_uring_submit(&ring);

    while (running) {
        io_uring_cqe* cqe = nullptr;

        if (io_uring_wait_cqe(&ring, &cqe) < 0) {
            spdlog::info( "ошибка wait_cqe\n");
            break;
        }

        auto* ctx = static_cast<OpContext*>(io_uring_cqe_get_data(cqe));
        pendingContexts.erase(
        std::remove(pendingContexts.begin(), pendingContexts.end(), ctx),
        pendingContexts.end()
        );
        int res = cqe->res;
        io_uring_cqe_seen(&ring, cqe);

        handlersMap[ctx->type](ctx, res);

        delete ctx;
    }
}
