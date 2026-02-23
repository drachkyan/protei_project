#include <iostream>
#include "../include/utils.h"
#include "../include/core.h"



int main(const int argc, char* argv[]) {

    App* app = nullptr;
    app = parseCommandArgs(argc, argv);
    if (app) {
        testInput(app);
    }
    delete app;
    mainLoop();


    // Vector<short> vector;
    // print_vector(vector);
    // input_vector(vector);
    // print_vector(vector);
    return 0;
}
