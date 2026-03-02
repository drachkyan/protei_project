#ifndef INCLUDE_RESOURCE_TEST_H
#define INCLUDE_RESOURCE_TEST_H

#include "../include/Test.h"
#include <unordered_map>
#include <string>
#include <vector>

class ResourceTest: public Test {
    std::vector<std::string> filesForTest;

    static bool fileExists(const std::string& path);
public:
    int test() override;

    explicit ResourceTest(const std::vector<std::string>& files_);
};


#endif  // INCLUDE_RESOURCE_TEST_H