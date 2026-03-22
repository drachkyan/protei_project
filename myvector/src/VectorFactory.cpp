#include "../include/VectorFactory.h"

#include <unordered_map>


std::optional<AnyVector> VectorFactory(size_t size_, const std::string &type_name) {
    auto& factoryMap = getFactoryMap();
    auto factory = factoryMap.find(type_name);
    if (factory == factoryMap.end()) {
        return std::nullopt;
    }
    return factory->second(size_);
}