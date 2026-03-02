#include "../include/ConnectionTest.h"

#include "spdlog/spdlog.h"

ConnectionTest::ConnectionTest(AppSettings& settings_): settings(settings_) {

}

inline int ConnectionTest::test() {
    if (!settings.verifySettings()) {
        spdlog::info("Невозможно подключиться - отстутствуют необходимые флаги");
        return 1;
    }
    return 0;
}
