#include "../include/Network/AppSettings.h"
#include "../include/Model/Menu.h"

int main(const int argc, char* argv[]) {

    auto app = AppSettings();
    app.parseCommandArgs(argc, argv);
    if (!app.verifySettings()) {
        return 0;
    }

    Menu menu(app);
    menu.run();
    return 0;
}
