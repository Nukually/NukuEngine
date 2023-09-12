// 在这个文件里写主效果实现逻辑
#include "Nuku.h"

// class HelloTriangleApplication {
// public:
//     void run() {
//         initWindow();
//         initVulkan();
//         mainLoop();
//         cleanup();
//     }

// private:
//     const uint32_t WIDTH = 800;
//     const uint32_t HEIGHT = 600;
//     GLFWwindow* window;
//     void initWindow() {
//         glfwInit();

//         glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//         glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

//         window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
//     }

//     void initVulkan() {

//     }

//     void mainLoop() {
//         while (!glfwWindowShouldClose(window)) {
//             glfwPollEvents();
//         }
//     }

//     void cleanup() {
//         glfwDestroyWindow(window);

//         glfwTerminate();
//     }
// };

int main() {
    NkEngineInit(800,600);
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