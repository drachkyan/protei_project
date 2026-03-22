#include "../include/Network/AppSettings.h"
#include "../include/Model/Menu.h"
#include "../include/utils/utils.h"

int main(const int argc, char* argv[]) {
    // test_out_of_bounds();
    auto app = AppSettings();
    app.parseCommandArgs(argc, argv);
    if (!app.verifySettings()) {
        return 0;
    }

    Menu menu(app);
    menu.run();
    return 0;
}
