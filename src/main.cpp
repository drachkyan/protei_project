#include <iostream>

#include "../include/AppSettings.h"
#include "../include/Menu.h"
#include "../include/DataPool.h"
#include "../include/utils.h"

void test_out_of_bounds() {
    std::vector<int> v(5);
    v[10] = 52;
}

int main(const int argc, char* argv[]) {
    // test_out_of_bounds();
    AppSettings app;
    app.parseCommandArgs(argc, argv);
    app.printSettings();
    test_util(app);

    std::unique_ptr<DataPool> data = std::make_unique<DataPool>();
    Menu menu(data);
    menu.run();
    return 0;
}
