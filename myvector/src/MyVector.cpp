#include "../include/MyVector.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

void MyVector::print() {
    if (!data) {
        spdlog::info("Ошибка вывода вектора - вектор неинициализирован");
        return;
    }
    std::visit([ ](const auto& vec) {
       for (int i = 0; i < vec.size(); i++) {
           std::cout << vec[i] <<" ";
       }
    }, *data);
    std::cout<< "\n";
}

void MyVector::toJson(json &j) {
    if (!data) {
        spdlog::info("Нельзя сделать JSON - вектор неинициализирован");
        return;
    }
    std::visit([&j, this](const auto& vec) {
        j["size"] = vec.size();
        j["type"] = type_name;
        j["data"] = vec;
    }, *data);
}

MyVector::MyVector(json &j) {
    size_t size_ = j["size"];
    type_name = j["type"].get<std::string>();
    data = VectorFactory(size_, type_name);
    if (!data) {
        spdlog::info("Ошибка парсинга вектора с жсона");
        return;
    }

    const auto& arr = j["data"];
    size_t idx = 0;

    std::visit([&](auto& vec) {
        using T = typename std::decay_t<decltype(vec)>::value_type;

        for (size_t a = 0; a < vec.size(); a++) {
            vec[a] = arr[idx++].get<T>();
        }

    }, *data);

}

MyVector::MyVector(const std::string &type_name_, size_t size_)  {
    type_name = std::move(type_name_);
    auto res = VectorFactory(size_, type_name);
    if (!res) {
        data = std::nullopt;
        return;
    }
    data = std::move(*res);
}


MyVector & operator*(MyVector &a_, MyVector &b_) {
    std::visit([ ](auto& a, auto& b) {
        if (a.size() != b.size()) {
            return;
        }
        for (int i = 0; i < a.size(); i++) {
            a[i]*=b[i];
        }
    }, *a_.getData(), *b_.getData());
    return a_;
}
