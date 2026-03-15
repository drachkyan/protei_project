#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H
#include "../Network/AppSettings.h"
#include "../Model/vector.hpp"
#include <nlohmann/json.hpp>
#include "../../include/utils/utils.h"
#include "Model/VectorFactory.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/chrono.h"
using json = nlohmann::json;

void test_util(AppSettings& settings);

void toLowerCase(std::string &c);

void anyVectorToJson(json& j, const AnyVector& v);

std::optional<AnyVector> vectorFromJson(const json& j);

#endif  // INCLUDE_UTILS_H
