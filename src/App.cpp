#include "App.h"
#include "Window.h"

#include <stdexcept>

Window::Window(int w, int h, const char *title)
    : width(w), height(h)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    // glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (handle == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // std::cout << "Decorated: " << glfwGetWindowAttrib(window, GLFW_DECORATED) << "\n";
    // std::cout << "Resizable: " << glfwGetWindowAttrib(window, GLFW_RESIZABLE) << "\n";
    // glfwSetWindowSizeLimits(window, 100, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

Window::~Window()
{
    glfwDestroyWindow(handle);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(handle);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(handle);
}

void Window::setResizeCallback(App *app)
{
    glfwSetWindowUserPointer(handle, app);
    glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app)
    {
        app->onResize(width, height);
    }
}

void Renderer::init()
{
}

void Renderer::render()
{
    glClearColor(1.0f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}