#ifndef INCLUDE_CONNECTION_TEST_H
#define INCLUDE_CONNECTION_TEST_H

#include "../Network/AppSettings.h"
#include "Test.h"


class ConnectionTest: public Test {
    AppSettings& settings;
public:
    ConnectionTest(AppSettings& settings);
    int test() override;
};

#endif  // INCLUDE_CONNECTION_TEST_H
