#ifndef INCLUDE_MENU_H
#define INCLUDE_MENU_H

#include <functional>
#include <unordered_map>
#include <string>
#include <utility>
#include <queue>
#include <variant>
#include "vector.hpp"
#include "../../include/Network/AppSettings.h"
#include "../../include/Network/NetworkClient.h"


using menu_func_type = std::function<u_int16_t()>;
using creator_func_type = std::function<void*()>;

class MenuItem {
public:
    menu_func_type action;
    std::string name;
    std::string description;
    MenuItem (menu_func_type action_, std::string name_, std::string description_):
    action (std::move(action_)), name (std::move(name_)), description(std::move(description_)) {};

    friend std::ostream& operator<<(std::ostream& os, const MenuItem& item) {
        if (item.description.empty()) { return os; }
        os << item.name << " - " << item.description << std::endl;
        return os;
    }
};


class Menu {
    std::queue<AnyVector> data;
    std::unique_ptr<NetworkClient> api;
    std::string alias;
    std::unordered_map<std::string, MenuItem> menuItems;
    std::shared_ptr<AppSettings> app{};
    void initMenuItems();

    static void inputVector(AnyVector &vector);
    void sendVector();
    void printVector();
    void addVector();
    static void quit();
public:

    Menu(const std::shared_ptr<AppSettings> &app_);

    friend std::ostream& operator<<(std::ostream& os, const Menu& menu) {
        os << "======== МЕНЮ =========" << std::endl;
        for (const auto& item: menu.menuItems) {
            os << item.second;
        }
        return os;
    }
    void run() const;
};



#endif  // INCLUDE_MENU_H
