#include "../../include/Model/Menu.h"
#include <iostream>
#include <string>
#include "spdlog/spdlog.h"
#include <queue>
#include "../../include/utils/utils.h"
#include "../../include/Network/NetworkClient.h"
#include "../../myvector/include/MyVector.hpp"
#include "Model/MenuItems/AddVectorItem.h"
#include "Model/MenuItems/PrintVectorItem.h"
#include "Model/MenuItems/QuitItem.h"
#include "Model/MenuItems/SendVectorItem.h"


void Menu::initMenuItems() {
    menuItems.insert({"quit",
        std::make_unique<QuitItem>("quit", "Выход из программы")});

    menuItems.insert({"exit",
        std::make_unique<QuitItem>("", "")});

    menuItems.insert({"add_vector",
        std::make_unique<AddVectorItem>("add_vector", "Добавление вектора", data)});

    menuItems.insert({"print_vector",
        std::make_unique<PrintVectorItem>("print_vector", "Вывод первого записанного вектора", data)});

    menuItems.insert({"send_vector",
       std::make_unique<SendVectorItem>("send_vector", "Отправить первый введенный вектор на сервер", data, api)});

}

Menu::Menu(AppSettings &app_): app(app_), api(app.getNetworkAddress().value()) {
    initMenuItems();

}

void Menu::run() const {
    if (!api.isConnected()) {
        spdlog::info("Соединение не установлено\n");
        return;
    }
    std::cout << *this;
    MENU_EXITS EXIT_CODE = MENU_EXITS::DEFAULT;
    std::string command;
    while (EXIT_CODE == MENU_EXITS::DEFAULT ) {

        if (!(std::cin >> command)) {
            spdlog::info("Соединение потеряно");
            break;
        }
        if (!api.isConnected()) {
            spdlog::info("Соединение потеряно");

            break;
        }
        toLowerCase(command);
        auto it = menuItems.find(command);
        if (it == menuItems.end()) {
            spdlog::info("Команда не найдена");
            continue;
        }
        EXIT_CODE = it->second->action();
    }
    fclose(stdin);
}
