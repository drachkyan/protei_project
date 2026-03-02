#include "../include/ResourceTest.h"
#include <filesystem>
#include <vector>

#include "spdlog/spdlog.h"

namespace fs = std::filesystem;

bool ResourceTest::fileExists(const std::string& path) {
    return fs::exists(path) && fs::is_regular_file(path);
}

int ResourceTest::test() {
    bool flag = true;
    for (auto& it : filesForTest) {
        if (fileExists(it)) {
            spdlog::info("Файл {} существует", it);
            continue;
        }
        spdlog::info("Файла {} не существует", it);
        flag = false;
    }
    if (flag) {
        return 0;
    }
    return 1;
}

ResourceTest::ResourceTest(const std::vector<std::string>& files_)
    : filesForTest(files_) {
}
