#pragma once

#include <variant>
#include <memory>
#include <nlohmann/json.hpp>
#include "VectorFactory.h"
#include <optional>

using json = nlohmann::json;


class MyVector {
    std::optional<AnyVector> data;
    std::string type_name;

public:
    std::optional<AnyVector>& getData(){return data;}
    std::string& getTypeName(){return type_name;}
    void print();
    void toJson(json& j);
    MyVector(json& j);

    friend MyVector& operator * (MyVector& a_, MyVector& b_);

    MyVector(const std::string& type_name, size_t size_);

    template <typename ElType>
    bool push(ElType el, size_t n) {
        bool flag = false;
        std::visit([&](auto& vec) {
            using T = typename std::decay_t<decltype(vec)>::value_type;
            if constexpr(std::is_integral_v<T> && std::is_floating_point_v<ElType>) {
                flag =  false;
                return;
            }
            vec[n] = el;
            flag =  true;
        }, *data);

        return flag;
    }


};

