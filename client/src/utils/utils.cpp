#include <vector>
#include "../../include/utils/utils.h"

#include "../../include/Network/AppSettings.h"
#include "../../include/Test/ConnectionTest.h"
#include "../../include/Test/ResourceTest.h"
#include "../../include/Test/Test.h"
#include <nlohmann/json.hpp>
#include <memory>

using json = nlohmann::json;

void test_util(AppSettings& settings) {
    std::vector<std::unique_ptr<Test>> tests{};
    tests.push_back(std::make_unique<ConnectionTest>(settings));

    tests.push_back(std::make_unique<ResourceTest>(std::vector<std::string>{
            "/mnt/c/Users/vova/Desktop/protei_hw/test.sh",
            "/mnt/c/Users/vova/Desktop/protei_hw/test123.sh"}));

    for (const auto& test : tests) {
        test->test();
    }
}

void toLowerCase(std::string &c) {
    size_t i = 0;
    for (size_t j = 0; j < c.size(); ++j) {
        c[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(c[i])));
        i++;
    }
}

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

