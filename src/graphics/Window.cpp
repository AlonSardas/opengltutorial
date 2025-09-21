
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
    glfwSetMouseButtonCallback(handle, mouseButtonCallback);
    glfwSetKeyCallback(handle, keyCallback);

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

void Window::setAppForCallback(App *app) { glfwSetWindowUserPointer(handle, app); }

void Window::setFullscreenMode(bool mode) {
    if (mode == fullscreenMode) {
        return;
    }

    fullscreenMode = mode;
    if (fullscreenMode) {
        // Store current state
        glfwGetWindowPos(handle, &windowedPosX, &windowedPosY);
        glfwGetWindowSize(handle, &windowedWidth, &windowedHeight);

        // Go borderless fullscreen
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_FALSE);
        glfwSetWindowPos(handle, 0, 0);
        glfwSetWindowSize(handle, mode->width, mode->height);
    } else {
        // Restore window
        glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_TRUE);
        glfwSetWindowPos(handle, windowedPosX, windowedPosY);
        glfwSetWindowSize(handle, windowedWidth, windowedHeight);
    }
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onResize(width, height);
    } else {
        std::cerr << "Could not find app instance to call onResize" << std::endl;
    }
}

void Window::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onMouseMove(xpos, ypos);
    } else {
        std::cerr << "Could not find app instance to call onMouseMove" << std::endl;
    }
}

void Window::mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onMouseScroll(xoffset, yoffset);
    } else {
        std::cerr << "Could not find app instance to call onMouseScroll" << std::endl;
    }
}

void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onMouseButton(button, action, mods);
    } else {
        std::cerr << "Could not find app instance to call onMouseButton" << std::endl;
    }
}

void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onKeyChanged(key, scancode, action, mods);
    } else {
        std::cerr << "Could not find app instance to call onKeyChanged" << std::endl;
    }
}
