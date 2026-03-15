#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

#include "../../client/include/Model/vector.hpp"

#include <nlohmann/json.hpp>
#include <optional>

using json = nlohmann::json;

void anyVectorToJson(json& j, const AnyVector& v);
std::optional<AnyVector> vectorFromJson(const json &j);

template <typename T>
T vectorSum(AnyVector& v) {
    T sum = 0;
    std::visit([&](auto& vec) {
        for (size_t a = 0; a < vec->m; a++)
            for (size_t b = 0; b < vec->n; b++)
                for (size_t c = 0; c < vec->i; c++)
                    for (size_t d = 0; d < vec->j; d++)
                        sum += vec->arr[a][b][c][d];
    }, v);
    return sum;
}

#endif  // INCLUDE_UTILS_H