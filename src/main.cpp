#include <iostream>
#include "../include/utils.h"

void testInput(const App* app) {
    std::cout << "Айпи адрес " << *app->addr << std::endl;
    std::cout << "Порт " << app->port << std::endl;
    std::cout << "Библиотека " << app->lib << std::endl;
    std::cout << "I " << app->i << std::endl;
    const char* role = app->role == Role::CLIENT ? "Client" : "Admin";
    std::cout << "Роль " << role << std::endl;
}

int main(const int argc, char* argv[]) {
    App* app = nullptr;
    app = parseCommandArgs(argc, argv);
    if (app) {
        testInput(app);
    }
    delete app;
    return 0;
}
