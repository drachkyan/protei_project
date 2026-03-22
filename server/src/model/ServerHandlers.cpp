#include "../../include/model/ServerHandlers.h"
#include <nlohmann/json.hpp>
#include "../../../myvector/include/MyVector.hpp"
#include <spdlog/spdlog.h>
#include "../include/utils/utils.h"

void ServerHandlers::initServerHandlersMap() {
    handlersMap["ping"] = &ServerHandlers::handlePing;
    handlersMap["vector"] = &ServerHandlers::handleVector;
}

void ServerHandlers::handlePing(json &req, json &res) {
    res["type"] = "pong";
}

void ServerHandlers::handleVector(json &req, json &res) {
    spdlog::info(req.dump());
    auto reqVector = MyVector(req);
    if (!reqVector.getData()) {
        spdlog::info("Ошибка парсинга вектора");
        return;
    }

    auto resVector = reqVector * reqVector;

    resVector.toJson(res);
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
