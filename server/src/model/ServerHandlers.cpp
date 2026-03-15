#include "../../include/model/ServerHandlers.h"
#include <nlohmann/json.hpp>
#include "../../client/include/Model/vector.hpp"
#include <spdlog/spdlog.h>
#include "../include/utils/utils.h"

void ServerHandlers::initServerHandlersMap() {
    handlersMap["ping"] = &ServerHandlers::handlePing;
    handlersMap["vector"] = &ServerHandlers::handleVector;
}

void ServerHandlers::handlePing(json &req, json &res) {
    res["type"] = "pong";
    spdlog::info("Понг");
}

void ServerHandlers::handleVector(json &req, json &res) {
    auto reqVector = vectorFromJson(req);
    if (!reqVector) {
        spdlog::info("Ошибка парсинга вектора");
        return;
    }
    AnyVector& vec = *reqVector;
    auto sum = vectorSum<double>(vec);

    res["result"] = sum;
}

void ServerHandlers::HandleJSON(json &req, json &res) {
    if (req.empty()) {
        spdlog::info("Получен пустой жсон");
        return;
    }
    if (!req.contains("type")) {
        spdlog::info("JSON в неверном формате");
        return;
    }
    std::string type = std::move(req["type"].get<std::string>());

    if (handlersMap.find(type) == handlersMap.end()) {
        type = "vector";
    }
    auto handler = handlersMap[type];
    (this->*handler)(req, res);
}
