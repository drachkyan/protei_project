#include "../../client/include/Model/vector.hpp"
#include "../../include/utils/utils.h"
#include <optional>
#include "../../client/include/Model/VectorFactory.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

void anyVectorToJson(json &j, const AnyVector &v) {
    std::visit([&j](const auto& vec) {
        vectorToJson(j, *vec);
        j["type"] = vec->type_name;
    }, v);
}

std::optional<AnyVector> vectorFromJson(const json &j) {
    size_t m = j["m"];
    size_t n = j["n"];
    size_t i = j["i"];
    size_t jj = j["j"];

    auto v = VectorFactory::create(j["type"], m, n, i, jj);
    if (!v) {
        spdlog::info("Ошибка создания");
        return std::nullopt;
    }
    const auto& arr = j["data"];
    size_t idx = 0;

    std::visit([&](auto& vec) {
        using T = typename std::decay_t<decltype(*vec)>::value_type;
        for (size_t a = 0; a < vec->m; a++)
            for (size_t b = 0; b < vec->n; b++)
                for (size_t c = 0; c < vec->i; c++)
                    for (size_t d = 0; d < vec->j; d++)
                        vec->arr[a][b][c][d] = arr[idx++].get<T>();
    }, *v);

    return std::move(*v);
}


void LoggerConfig() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("server.log", true);
    auto logger = std::make_shared<spdlog::logger>("server",
        spdlog::sinks_init_list{console_sink, file_sink});

    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);

}