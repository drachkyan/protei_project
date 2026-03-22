#include "../include/network/Server.h"
#include "../include/utils/utils.h"

void startServer(int argc, char* argv[]) {
    auto port = parseCommandArgs(argc, argv);
    if (port == 0) {
        return;
    }
    if (port==-1) {
        spdlog::info("Неверные аргументы запуска");
        return;
    }
    Server server(port);
    if (server.isCorrect()) {
        server.run();
    }
}

int main(int argc, char* argv[]) {
    LoggerConfig();
    startServer(argc, argv);
    return 0;
}