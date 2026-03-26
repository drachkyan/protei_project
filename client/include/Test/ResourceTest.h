#pragma once

#include "Test.h"
#include <string>
#include <vector>

class ResourceTest: public Test {
    std::vector<std::string> filesForTest;

    static bool fileExists(const std::string& path);
public:
    int test() override;

    explicit ResourceTest(const std::vector<std::string>& files_);
};
