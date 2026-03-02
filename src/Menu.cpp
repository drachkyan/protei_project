#include "../include/Menu.h"
#include <iostream>
#include <string>
#include "spdlog/spdlog.h"

void toLowerCase(std::string c) {
    size_t i = 0;
    while (c[i] != '\0') {
        c[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(c[i])));
        i++;
    }
}

void Menu::quit() {
    spdlog::info("Выход из программы");
}

Types Menu::inputType() {
    std::cout << "Введите тип\n";
    constexpr u_int16_t BUFFER_SIZE  = 10;
    char buffer[BUFFER_SIZE]{};
    std::cin >> buffer;
    auto it = typesMap.find(buffer);
    if (it == typesMap.end()) {
        return Types::None;
    }
    return it->second;

}

void * Menu::createVectorByType(Types type) {
    auto it = vectorCreators.find(type);
    if (it != vectorCreators.end()) {
        return it->second();
    }
    return nullptr;
}

void Menu::inputVector(Types type, void *&vectorPtr) {
    std::cout << "Введите вектор 2х2х2х2\n";
    dispatch(type, vectorPtr, [](auto* v) {
        input_vector(*v);
    });
}

void Menu::printVector() const {
    auto [vector, type] = data->first();

    dispatch(type, vector, [](auto* v) {
        print_vector(*v);
    });

    dispatch(type, vector, [](auto* v) {
        delete v;
    });
}

void Menu::addVector() {
    Types type = inputType();
    if (type==Types::None) {
        spdlog::info("Тип не найден");
        return;
    }
    void* vector = createVectorByType(type);
    if (!vector) {
        spdlog::info("Ошибка создания вектора");
        return;
    }
    inputVector(type, vector);
    data->insert(vector, type);
}

void Menu::inputAlias() {
    std::cout << "Введите строковой псеводоним до 10 символов\n";
    std::cin >> alias;
}

void Menu::initMenuItems() {
    menuItems.insert({"quit",
        MenuItem([](){Menu::quit();return 1;},"quit","Выход из программы")});

    menuItems.insert({"add_vector",
        MenuItem([this](){this->addVector();return 0;},"add_vector","Добавить вектор")});

    menuItems.insert({"input_alias",
        MenuItem([this](){this->inputAlias();return 0;}, "input_alias", "Ввод строкового псевдонима")});

    menuItems.insert({"print_vector",
        MenuItem([this](){this->printVector();return 0;}, "print_vector","Вывод первого записанного вектора с удалением из очереди")});
}

void Menu::initTypes() {
    typesMap["u_int64_t"] = Types::U_INT64_T;
    typesMap["u_int16_t"] = Types::U_INT16_T;
    typesMap["u_int32_t"] = Types::U_INT;
    typesMap["int64_t"]   = Types::INT_64_T;
    typesMap["int16_t"]   = Types::INT_16_T;
    typesMap["int32_t"]   = Types::INT_T;
    typesMap["float"]     = Types::FLOAT;
    typesMap["char"]      = Types::CHAR;
    typesMap["double"]    = Types::DOUBLE;
}

void Menu::initCreators() {
    vectorCreators[Types::U_INT64_T] = []() -> void* { return new Vector<u_int64_t>; };
    vectorCreators[Types::U_INT16_T] = []() -> void* { return new Vector<u_int16_t>; };
    vectorCreators[Types::U_INT] = []() -> void* { return new Vector<u_int32_t>; };
    vectorCreators[Types::INT_64_T] = []() -> void* { return new Vector<int64_t>; };
    vectorCreators[Types::INT_16_T] = []() -> void* { return new Vector<int16_t>; };
    vectorCreators[Types::INT_T] = []() -> void* { return new Vector<int32_t>; };
    vectorCreators[Types::FLOAT] = []() -> void* { return new Vector<float>; };
    vectorCreators[Types::CHAR] = []() -> void* { return new Vector<char>; };
    vectorCreators[Types::DOUBLE] = []() -> void* { return new Vector<double>; };
}

Menu::Menu(DataPool*data_):data(data_) {
    initTypes();
    initMenuItems();
    initCreators();
}

void Menu::run() const {
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
