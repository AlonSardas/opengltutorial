#pragma once

#include <glad/glad.h> // Must come before glfw3.h

#include <GLFW/glfw3.h>

class App;

class Window {
  public:
    GLFWwindow *handle;

    Window(int w, int h, const char *title);
    ~Window();

    void pollEvents();
    void swapBuffers();
    void setResizeCallback(App *appInstance);
    bool shouldClose() const;
    inline int getKeyStatus(int key) { return glfwGetKey(handle, key); }
    inline void setShouldClose(bool val) {
        glfwSetWindowShouldClose(handle, val);
    }

  private:
    static void framebufferSizeCallback(GLFWwindow *window, int width,
                                        int height);
};
