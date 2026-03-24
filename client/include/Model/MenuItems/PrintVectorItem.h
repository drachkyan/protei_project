#pragma once
#include "../MenuItem.h"

class PrintVectorItem : public AbstractMenuItem {
    std::queue<MyVector>& data;

public:
    PrintVectorItem (std::string name_, std::string description_, std::queue<MyVector>& data_):
        AbstractMenuItem(std::move(name_), std::move(description_)), data(data_)  {};

    MENU_EXITS action() override {
        if (data.empty()) {
            spdlog::info("Еще не записывались векторы");
            return MENU_EXITS::DEFAULT;
        }
        data.front().print();
        return MENU_EXITS::DEFAULT;
    }
};