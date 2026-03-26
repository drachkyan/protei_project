#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

void LoggerConfig();
uint16_t parsePort(const char *str);
size_t hash(const char* str);
int32_t parseCommandArgs(int argc, char* argv[]);


void startServer(int argc, char* argv[]);