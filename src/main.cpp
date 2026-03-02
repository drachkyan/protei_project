#include <iostream>

#include "../include/AppSettings.h"
#include "../include/Menu.h"
#include "../include/DataPool.h"
#include "../include/utils.h"

int main(const int argc, char* argv[]) {
    AppSettings app;
    app.parseCommandArgs(argc, argv);
    app.printSettings();
    test_util(app);

    DataPool data;
    Menu menu(&data);
    menu.run();
    return 0;
}
