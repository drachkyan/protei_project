#include "../include/Network/AppSettings.h"
#include "../include/Model/Menu.h"
#include "../include/utils/utils.h"

void test_out_of_bounds() {
    std::vector<int> v(5);
    v[10] = 52;
}

int main(const int argc, char* argv[]) {
    // test_out_of_bounds();
    auto app = std::make_shared<AppSettings>();
    app->parseCommandArgs(argc, argv);
    app->printSettings();
    // test_util(app);

    Menu menu(app);
    menu.run();
    return 0;
}
