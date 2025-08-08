#pragma once

#include <glad/glad.h> // Must come before glfw3.h
#include <GLFW/glfw3.h>

class App;

class Window
{
public:
    GLFWwindow *handle;
    int width, height;

    Window(int w, int h, const char *title);
    ~Window();

    void pollEvents();
    void swapBuffers();
    void setResizeCallback(App *appInstance);
    bool shouldClose() const;

private:
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};
