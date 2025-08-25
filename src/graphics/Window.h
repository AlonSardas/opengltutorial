#pragma once

#include <glad/glad.h> // Must come before glfw3.h

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

class App;

class Window {
  public:
    GLFWwindow *handle;

    Window(int w, int h, const char *title);
    ~Window();

    void pollEvents();
    void swapBuffers();
    void setAppForCallback(App *appInstance);
    void setFullscreenMode(bool mode);
    bool shouldClose() const;
    int getKeyStatus(int key) { return glfwGetKey(handle, key); }
    void setShouldClose(bool val) { glfwSetWindowShouldClose(handle, val); }
    bool isFullscreen() { return fullscreenMode; }
    void toggleFullscreen() { setFullscreenMode(!fullscreenMode); }

  private:
    bool fullscreenMode = false;
    int windowedPosX = 0, windowedPosY = 0, windowedWidth = 0, windowedHeight = 0;

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
};
