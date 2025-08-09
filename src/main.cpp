#include "App.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    try {
        App app;
        app.run();
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwTerminate();
    return 0;
}