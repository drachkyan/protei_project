#include "../include/core.h"
#include <iostream>
#include "../include/utils.h"
#include "../include/vector.hpp"


void core_delete_vector(Types type, void* vectorPtr) {
    if (type == Types::None) {
        return;
    }
    dispatch(type, vectorPtr, [](auto* v) {
        delete v;
    });
}


Types input_type(Types type, void* &vectorPtr) {
    std::cout << "Введите тип\n";
    constexpr u_int16_t BUFFER_SIZE  = 10;
    char buffer[BUFFER_SIZE]{};
    std::cin >> buffer;
    if (vectorPtr) {
        core_delete_vector(type, vectorPtr);
    }
    switch (hash(buffer)) {
        case hash("u_int64_t"):
            vectorPtr = new Vector<u_int64_t>;
            return Types::U_INT64_T;
        case hash("u_int16_t"):
            vectorPtr = new Vector<u_int16_t>;
            return Types::U_INT16_T;
        case hash("u_int32_t"):
            vectorPtr = new Vector<u_int32_t>;
            return Types::U_INT;
        case hash("int64_t"):
            vectorPtr = new Vector<int64_t>;
            return Types::INT_64_T;
        case hash("int16_t"):
            vectorPtr = new Vector<int16_t>;
            return Types::INT_T;
        case hash("int32_t"):
            vectorPtr = new Vector<int32_t>;
            return Types::INT_T;
        case hash("float"):
            vectorPtr = new Vector<float>;
            return Types::FLOAT;
        case hash("char"):
            vectorPtr = new Vector<char>;
            return Types::CHAR;
        case hash("double"):
            vectorPtr = new Vector<double>;
            return Types::DOUBLE;
        default:
            std::cout << "Данный тип вектора не поддерживается\n";
            return Types::None;
    }
}

void printMenu() {
    std::cout <<
        "==== МЕНЮ ====\n" <<
        "input_alias - ввод строкового псевдонима\n"<<
        "input_type - введите тип вектора\n" <<
        "input_vector - ввод четырехмерного вектора\n" <<
        "print_vector - вывод четырехмерного вектора\n" <<
        "quit - выход\n";
}



void input_alias(char*alias) {
    std::cout << "Введите строковой псеводоним до 10 символов\n";
    std::cin >> alias;
}

void core_print_vector(const Types type, void* &vectorPtr) {
    if (type == Types::None) {
        std::cout << "Сначала введите тип данных\n";
        return;
    }
    dispatch(type, vectorPtr, [](auto* v) {
        print_vector(*v);
    });
}

void core_input_vector(const Types type, void* &vectorPtr) {
    if (type == Types::None) {
        std::cout << "Сначала введите тип данных\n";
        return;
    }
    dispatch(type, vectorPtr, [](auto* v) {
        input_vector(*v);
    });
}

void mainLoop() {
    constexpr u_int16_t BUFFER_SIZE  = 256, ALIAS_SIZE = 10;
    char command[BUFFER_SIZE], alias[ALIAS_SIZE];
    auto type = Types::None;
    printMenu();
    std::cin >> command;
    void* vectorPtr = nullptr;
    while (hash(command) != hash("quit")) {
        switch (hash(command)) {
            case hash("input_alias"):
                input_alias(alias);
                break;
            case hash("input_type"):
                type = input_type(type, vectorPtr);
                break;
            case hash("input_vector"):
                core_input_vector(type, vectorPtr);
                break;
            case hash("print_vector"):
                core_print_vector(type, vectorPtr);
                break;
            default:
                std::cout << "Неизвестная комманда\n";
                break;
        }
        std::cin >> command;
    }
    std::cout << "Выход\n";
    core_delete_vector(type, vectorPtr);
}
