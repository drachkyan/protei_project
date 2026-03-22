#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

#include "../../myvector/include/MyVector.hpp"

#include <nlohmann/json.hpp>
#include <optional>

using json = nlohmann::json;

void LoggerConfig();
uint16_t parsePort(const char *str);
size_t hash(const char* str);
int32_t parseCommandArgs(int argc, char* argv[]);

#endif  // INCLUDE_UTILS_H