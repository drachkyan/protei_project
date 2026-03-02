#ifndef INCLUDE_CONNECTION_TEST_H
#define INCLUDE_CONNECTION_TEST_H

#include "../include/AppSettings.h"
#include "../include/Test.h"


class ConnectionTest: public Test {
    AppSettings& settings;
public:
    ConnectionTest(AppSettings& settings);
    int test() override;
};



#endif  // INCLUDE_CONNECTION_TEST_H
