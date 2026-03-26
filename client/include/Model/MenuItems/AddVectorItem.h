#pragma once
#include "../MenuItem.h"

class AddVectorItem : public AbstractMenuItem {
    std::queue<MyVector>& data;
    bool inputVector(MyVector& vector, size_t size_) {
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
        spdlog::info("Вектор введен успешно");
        std::cin.ignore(10000, '\n');
        return true;
    }
public:
    AddVectorItem (std::string name_, std::string description_, std::queue<MyVector>& data_):
        AbstractMenuItem(std::move(name_), std::move(description_)), data(data_)  {};

    MENU_EXITS action() override {
        int32_t size_ = 0;
        std::string type_name{};
        std::cout<<"Введите тип вектора \n";
        std::cin>>type_name; toLowerCase(type_name);
        std::cout<<"Введите размер вектора \n";

        if (! (std::cin >> size_) ) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            spdlog::info("Размер вектора задан не верно");
            return MENU_EXITS::DEFAULT;
        }

        if (size_ < 0 || size_ > 65536) {
            spdlog::info("Размер вектора задан не верно");
            return MENU_EXITS::DEFAULT;
        }

        auto vector = MyVector(type_name, size_);  // тут приведение типа, но размер ограничен до 2^16
        // поэтому будет работать (чтоб не делать доп проверки)
        if (!vector.getData()) {
            spdlog::info("Ошибка создания вектора");
            return MENU_EXITS::DEFAULT;
        }
        if (! inputVector(vector, size_)) {
            spdlog::info("Ошибка создания вектора");
            return MENU_EXITS::DEFAULT;
        }
        data.push(vector);
        return MENU_EXITS::DEFAULT;
    }
};