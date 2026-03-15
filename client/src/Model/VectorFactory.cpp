#include "../../include/Model/VectorFactory.h"

#include <unordered_map>


VectorFactory::VectorFactory() = default;


std::optional<AnyVector> VectorFactory::create(const std::string &type_name,
                                                size_t m, size_t n, size_t i, size_t j) {
    if (type_name == "u_int64_t") return std::make_unique<Vector<uint64_t>>(type_name, m, n, i, j);
    if (type_name == "u_int16_t") return std::make_unique<Vector<uint16_t>>(type_name, m, n, i, j);
    if (type_name == "u_int32_t") return std::make_unique<Vector<uint32_t>>(type_name, m, n, i, j);
    if (type_name == "int64_t")   return std::make_unique<Vector<int64_t>> (type_name, m, n, i, j);
    if (type_name == "int16_t")   return std::make_unique<Vector<int16_t>> (type_name, m, n, i, j);
    if (type_name == "int")       return std::make_unique<Vector<int32_t>> (type_name, m, n, i, j);
    if (type_name == "float")     return std::make_unique<Vector<float>>   (type_name, m, n, i, j);
    if (type_name == "double")    return std::make_unique<Vector<double>>  (type_name, m, n, i, j);
    if (type_name == "char")      return std::make_unique<Vector<char>>    (type_name, m, n, i, j);
    return std::nullopt;
}