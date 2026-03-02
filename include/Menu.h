#ifndef INCLUDE_MENU_H
#define INCLUDE_MENU_H

#include <functional>
#include <unordered_map>
#include <string>
#include <utility>
#include <iostream>
#include "../include/DataPool.h"


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
        os << item.name << " - " << item.description;
        return os;
    }
};


class Menu {
    DataPool* data;
    std::string alias;
    std::unordered_map<std::string, MenuItem> menuItems;
    std::unordered_map<std::string, Types> typesMap;
    std::unordered_map<Types, creator_func_type> vectorCreators;

    void initMenuItems();
    void initTypes();
    void initCreators();

    Types inputType();
    void* createVectorByType(Types type);
    static void inputVector(Types type, void* &vectorPtr);

    void printVector() const;
    void addVector();
    void inputAlias();
    static void quit();
public:

    Menu(DataPool *data_);

    friend std::ostream& operator<<(std::ostream& os, const Menu& menu) {
        os << "======== МЕНЮ =========" << std::endl;
        for (const auto& item: menu.menuItems) {
            os << item.second << std::endl;
        }
        return os;
    }
    void run() const;
};



#endif  // INCLUDE_MENU_H
