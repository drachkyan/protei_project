#include <iostream>

#include "../include/AppSettings.h"
#include "../include/core.h"
#include <spdlog/spdlog.h>


int main(const int argc, char* argv[]) {
    AppSettings app;
    spdlog::info("конструктор сделался");
    app.parseCommandArgs(argc, argv);
    app.printSettings();
    // Vector<short> vector;
    // print_vector(vector);
    // input_vector(vector);
    // print_vector(vector);
    return 0;
}
