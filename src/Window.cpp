#include "Window.h"
#include "App.h"

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

    // Callbacks
    glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);
    glfwSetCursorPosCallback(handle, mouseMoveCallback);
    glfwSetScrollCallback(handle, mouseScrollCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // if (glfwRawMouseMotionSupported()) {
    //     std::cout << "raw movement support" << std::endl;
    //     glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    // }

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

void Window::setAppForCallback(App *app) {
    glfwSetWindowUserPointer(handle, app);
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onResize(width, height);
    } else {
        std::cerr << "Could not find app instance to call onResize"
                  << std::endl;
    }
}

void Window::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onMouseMove(xpos, ypos);
    } else {
        std::cerr << "Could not find app instance to call onMouseMove"
                  << std::endl;
    }
}

void Window::mouseScrollCallback(GLFWwindow *window, double xoffset,
                                 double yoffset) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onMouseScroll(xoffset, yoffset);
    } else {
        std::cerr << "Could not find app instance to call onMouseScroll"
                  << std::endl;
    }
}
