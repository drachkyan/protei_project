#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H
#include "../Network/AppSettings.h"
#include "../../../myvector/include/MyVector.hpp"
#include <nlohmann/json.hpp>
#include "../../include/utils/utils.h"
#include "spdlog/spdlog.h"

using json = nlohmann::json;

void test_util(AppSettings& settings);

void toLowerCase(std::string &c);

size_t hash(const char* str);

#endif  // INCLUDE_UTILS_H
