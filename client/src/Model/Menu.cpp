#include "../../include/Model/Menu.h"
#include <iostream>
#include <string>
#include "spdlog/spdlog.h"
#include <queue>
#include "../../include/utils/utils.h"
#include "../../include/Network/NetworkClient.h"
#include "../../myvector/include/MyVector.hpp"

void Menu::quit() {
    spdlog::info("Выход из программы");
}


bool Menu::inputVector(MyVector& vector, size_t size_) {
    std::cout << "Введите вектор\n";
    std::string str_num{};
    for (size_t i = 0; i < size_; i++) {
        std::cin>>str_num;
        try {
            if (str_num.find('.') != std::string::npos) {
                if ( !vector.push(std::stod(str_num), i) ) {
                    spdlog::info("Неверный формат данных для вектора");
                    std::cin.ignore(10000, '\n');
                    return false;
                }
            }else {
                vector.push(std::stoi(str_num), i);
            }
        }
        catch (const std::exception& e) {
            spdlog::info("Неверный формат числа");
            std::cin.ignore(10000, '\n');
            return false;
        }
    }
    std::cin.ignore(10000, '\n');
    return true;
}

void Menu::sendVector() {
    if (data.empty()) {
        spdlog::info("Еще не записывались векторы");
        return;
    }
    json j{};
    auto& vector = data.front();

    vector.toJson(j);

    if (j.empty()) {
        spdlog::info("Ошибка конвертации вектора в json\n");
        return;
    }
    api->sendJSON(j);
    auto res = api->recvJSON();
    std::cout << "Результат: ";
    MyVector(res).print();
    data.pop();
}

void Menu::printVector() {
    if (data.empty()) {
        spdlog::info("Еще не записывались векторы");
        return;
    }
    data.front().print();
}



void Menu::addVector() {
    int32_t size_ = 0;
    std::string type_name{};
    std::cout<<"Введите тип вектора \n";
    std::cin>>type_name; toLowerCase(type_name);
    std::cout<<"Введите размер вектора \n";

    if (! (std::cin >> size_) ) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        spdlog::info("Размер вектора задан не верно");
        return;
    }

    if (size_ < 0 || size_ > 65536) {
        spdlog::info("Размер вектора задан не верно");
        return;
    }

    auto vector = MyVector(type_name, size_);  // тут приведение типа, но размер ограничен до 2^16
                                                        // поэтому будет работать (чтоб не делать доп проверки)
    if (!vector.getData()) {
        spdlog::info("Ошибка создания вектора");
        return;
    }
    if (! inputVector(vector, size_)) {
        spdlog::info("Ошибка создания вектора");
        return;
    }
    data.push(vector);
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
    while (EXIT_CODE == 0 ) {

        if (!(std::cin >> command)) {
            spdlog::info("Соединение потеряно");
            break;
        }
        if (!api->isConnected()) {
            spdlog::info("Соединение потеряно");

            break;
        }
        toLowerCase(command);
        auto it = menuItems.find(command);
        if (it == menuItems.end()) {
            spdlog::info("Команда не найдена");
            continue;
        }
        EXIT_CODE = it->second.action();
    }
    fclose(stdin);
}
