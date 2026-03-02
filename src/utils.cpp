#include <vector>
#include "../include/utils.h"

#include "../include/AppSettings.h"
#include "../include/ConnectionTest.h"
#include "../include/ResourceTest.h"
#include "../include/Test.h"

void test_util(AppSettings& settings) {
    const std::vector<Test*> tests = {
        new ResourceTest(std::vector<std::string>{
            "/mnt/c/Users/vova/Desktop/protei_hw/test.sh",
            "/mnt/c/Users/vova/Desktop/protei_hw/test123.sh"}),
        new ConnectionTest(settings)
    };
    for (const auto test : tests) {
        test->test();
        delete test;
    }
}