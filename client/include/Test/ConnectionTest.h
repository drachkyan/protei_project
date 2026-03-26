#pragma once

#include "../Network/AppSettings.h"
#include "Test.h"


class ConnectionTest: public Test {
    AppSettings& settings;
public:
    ConnectionTest(AppSettings& settings);
    int test() override;
};
