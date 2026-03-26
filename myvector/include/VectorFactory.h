#pragma once


#include <unordered_map>
#include <vector>
#include <cstdint>
#include <variant>
#include <functional>
#include <string>
#include <optional>

using AnyVector = std::variant<
    std::vector<uint64_t>,
    std::vector<uint16_t>,
    std::vector<uint32_t>,
    std::vector<int64_t>,
    std::vector<int16_t>,
    std::vector<int32_t>,
    std::vector<float>,
    std::vector<double>,
    std::vector<char>
>;

using factory_func = std::function<AnyVector(size_t)>;

static const std::unordered_map<std::string, factory_func>& getFactoryMap() {
    static const std::unordered_map<std::string, factory_func> factoryMap = {
        {"uint64_t", [](size_t s) { return std::vector<uint64_t>(s); }},
        {"uint16_t", [](size_t s) { return std::vector<uint16_t>(s); }},
        {"uint32_t", [](size_t s) { return std::vector<uint32_t>(s); }},
        {"int64_t", [](size_t s) { return std::vector<int64_t>(s); }},
        {"int16_t", [](size_t s) { return std::vector<int16_t>(s); }},
        {"int", [](size_t s) { return std::vector<int32_t>(s); }},
        {"float", [](size_t s) { return std::vector<float>(s); }},
        {"double", [](size_t s) { return std::vector<double>(s); }},
        {"char", [](size_t s) { return std::vector<char>(s); }},
    };
    return factoryMap;
}


std::optional<AnyVector> VectorFactory(size_t size_, const std::string& type_name);