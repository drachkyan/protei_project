#pragma once
#include "../MenuItem.h"
#include "../../client/include/Network/NetworkClient.h"

class SendVectorItem : public AbstractMenuItem {
    std::queue<MyVector>& data;
    NetworkClient&api;
public:
    SendVectorItem (std::string name_, std::string description_, std::queue<MyVector>& data_, NetworkClient&api_):
        AbstractMenuItem(std::move(name_), std::move(description_)), data(data_), api(api_)   {};

    MENU_EXITS action() override {
        if (data.empty()) {
            spdlog::info("Еще не записывались векторы");
            return MENU_EXITS::DEFAULT;
        }
        json j{};
        auto& vector = data.front();

        vector.toJson(j);

        if (j.empty()) {
            spdlog::info("Ошибка конвертации вектора в json\n");
            return MENU_EXITS::DEFAULT;
        }
        json res{};
        api.sendRecv(j, res);
        std::cout << "Результат: ";
        MyVector(res).print();
        data.pop();
        return MENU_EXITS::DEFAULT;
    }
};
