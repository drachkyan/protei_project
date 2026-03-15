#include "../../include/Model/Menu.h"
#include <iostream>
#include <string>
#include "spdlog/spdlog.h"
#include <queue>
#include "../../include/Model/vector.hpp"
#include "../../include/Model/VectorFactory.h"
#include "../../include/utils/utils.h"
#include "../../include/Network/NetworkClient.h"

void Menu::quit() {
    spdlog::info("Выход из программы");
}


void Menu::inputVector(AnyVector& vector) {
    std::cout << "Введите вектор 2х2х2х2\n";
    std::visit([](auto& v) {
        input_vector(*v);
    }, vector);
}

void Menu::sendVector() {
    if (data.empty()) {
        spdlog::info("Еще не записывались векторы");
        return;
    }
    spdlog::info("Начало отправки вектора\n");
    api->sendVector(data.front());
    data.pop();
}

void Menu::printVector() {
    if (data.empty()) {
        spdlog::info("Еще не записывались векторы");
        return;
    }

    std::visit([](auto& v) {
        print_vector(*v);
    }, data.front());
}



void Menu::addVector() {
    std::string type_name{};
    std::cout<<"Введите тип вектора \n";
    std::cin>>type_name; toLowerCase(type_name);

    auto vector = VectorFactory().create(type_name);
    if (!vector) {
        spdlog::info("Ошибка создания вектора");
        return;
    }
    inputVector(*vector);
    data.push(std::move(*vector));
}


void Menu::initMenuItems() {
    menuItems.insert({"quit",
        MenuItem([](){Menu::quit();return 1;},"quit","Выход из программы")});

    menuItems.insert({"exit",
        MenuItem([](){Menu::quit();return 1;},"","")});

    menuItems.insert({"add_vector",
        MenuItem([this](){this->addVector();return 0;},"add_vector","Добавить вектор")});

    menuItems.insert({"send_vector",
       MenuItem([this](){this->sendVector();return 0;},"send_vector","Отправить первый введенный вектор на сервер")});

    menuItems.insert({"print_vector",
        MenuItem([this](){this->printVector();return 0;}, "print_vector","Вывод первого записанного вектора")});
}

Menu::Menu(const std::shared_ptr<AppSettings> &app_): app(app_) {
    initMenuItems();
    api = std::make_unique<NetworkClient>(app->getNetworkAddress());

}

void Menu::run() const {
    if (!api->isConnected()) {
        spdlog::info("Соединение не установлено\n");
        return;
    }
    std::cout << *this;
    u_int16_t EXIT_CODE = 0;
    std::string command;
    while (EXIT_CODE == 0) {
        std::cin >> command;
        toLowerCase(command);
        auto it = menuItems.find(command);
        if (it == menuItems.end()) {
            spdlog::info("Команда не найдена");
            continue;
        }
        EXIT_CODE = it->second.action();
    }
}
