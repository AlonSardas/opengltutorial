/*
https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/6.1.coordinate_systems/coordinate_systems.cpp

https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/6.3.coordinate_systems_multiple/coordinate_systems_multiple.cpp
*/
#include "App.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include <stdexcept>

void App::run() {
    while (!window.shouldClose()) {
        processInput();

        renderer.render();

        window.swapBuffers();
        glfwPollEvents();
    }
}

void App::processInput() {
    if (window.getKeyStatus(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        window.setShouldClose(true);
    }
}

Window::Window(int w, int h, const char *title) {
    int width = w;
    int height = h;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    // glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (handle == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(handle);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // std::cout << "Decorated: " << glfwGetWindowAttrib(window, GLFW_DECORATED)
    // << "\n"; std::cout << "Resizable: " << glfwGetWindowAttrib(window,
    // GLFW_RESIZABLE) << "\n"; glfwSetWindowSizeLimits(window, 100, 100,
    // GLFW_DONT_CARE, GLFW_DONT_CARE);
}

Window::~Window() { glfwDestroyWindow(handle); }

bool Window::shouldClose() const { return glfwWindowShouldClose(handle); }

void Window::swapBuffers() { glfwSwapBuffers(handle); }

void Window::setResizeCallback(App *app) {
    glfwSetWindowUserPointer(handle, app);
    glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onResize(width, height);
    }
}

App::App() : window(800, 600, "My openGL app") {
    renderer.init();
    renderer.onResize(800, 600);
    window.setResizeCallback(this);
}
