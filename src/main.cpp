#include <iostream>

#include "../include/AppSettings.h"
#include "../include/Menu.h"
#include "../include/DataPool.h"

int main(const int argc, char* argv[]) {
    AppSettings app;
    app.parseCommandArgs(argc, argv);
    app.printSettings();

    DataPool data;
    Menu menu(&data);
    menu.run();
    return 0;
}
