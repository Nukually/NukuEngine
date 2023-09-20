#include "Nuku.h"
#include"NkEngine.hpp"

int main() {
    nk::Engine engine{};
    engine.run();
    /*NkEngineInit(800, 600);
    NkEngineRender();
    NkEngineClearUp();*/
    return 0;
    // try {
    //     app.run();
    // }
    // catch (const std::exception& e) {
    //     std::cerr << e.what() << std::endl;
    //     return EXIT_FAILURE;
    // }

    // return EXIT_SUCCESS;
}