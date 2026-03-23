#pragma once

#include <nlohmann/json.hpp>
#include "../../../myvector/include/MyVector.hpp"

using json = nlohmann::json;

class ServerHandlers {
    using HandlerPtr = void (ServerHandlers::*)(json&, json&);

    std::unordered_map<std::string, HandlerPtr> handlersMap;

public:

    void initServerHandlersMap();

    void handlePing(json& req, json& res);
    void handleVector(json& req, json& res);
    void HandleJSON(json& req, json& res);

    ServerHandlers() {
        initServerHandlersMap();
    }
};
