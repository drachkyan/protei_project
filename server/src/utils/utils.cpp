#include "../../../myvector/include/MyVector.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>


void LoggerConfig() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("server.log", true);
    auto logger = std::make_shared<spdlog::logger>("server",
        spdlog::sinks_init_list{console_sink, file_sink});

    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);

}

uint16_t parsePort(const char *str) {
    char* end = nullptr;

    const int64_t tempPort = strtol(str, &end, 10);

    if (*end != '\0') {
        spdlog::info("Ошибка при парсинге порта");
        return 0;
    }
    if (constexpr int64_t MAX_PORT = 65535;
        tempPort > MAX_PORT || tempPort <= 0) {
        spdlog::info("Порт должен быть в диапазоне 1 - 65535");
        return 0;
        }
    return tempPort;
}

size_t hash(const char* str) {
    size_t hash = 0;
    while (*str) {
        hash = hash * 31 + *str;
        ++str;
    }
    return hash;
}

int32_t parseCommandArgs(int argc, char* argv[]) {
    if (argc > 3 || argc < 1) {
        spdlog::info("Неверное количество аргументов при запуске");
        return -1;
    }
    if (argc == 1) {
        return 8080;
    }
    if (hash(argv[1]) == hash("--help") && argc == 2) {
        std::cout << "Сервер, принимает жсон с массивом и возводит каждый элемент в квадрат\n"
        << "-p : Ввод порта (дефолтный 8080)\n"
        << "--help : Вывод справки\n";
        return 0;
    }
    int32_t port = -1;
    if (argc == 3 && hash(argv[1]) == hash("-p")) {
        port = parsePort(argv[2]);
        if (port == 0) {
            spdlog::info("Введен порт 0 или нечитаемый порт - применяется порт по умолчанию (8080)");
            return 8080;
        }
    }
    return port;

}
