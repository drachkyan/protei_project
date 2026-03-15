#ifndef INCLUDE_VECTOR_FACTORY_H
#define INCLUDE_VECTOR_FACTORY_H

#include <unordered_map>

#include "vector.hpp"
#include <optional>

class VectorFactory {
public:
    static std::optional<AnyVector> create(const std::string &type_name,
                                            size_t m = 2, size_t n = 2, size_t i = 2, size_t j = 2);
    VectorFactory();
};

#endif  // INCLUDE_VECTOR_FACTORY_H