#include "../include/network/Server.h"
#include "../include/utils/utils.h"

int main() {
    LoggerConfig();
    Server server(8080);
    server.run();
    return 0;
}