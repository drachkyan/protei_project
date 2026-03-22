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


size_t hash(const char* str) {
    size_t hash = 0;
    while (*str) {
        hash = hash * 31 + *str;
        ++str;
    }
    return hash;
}
