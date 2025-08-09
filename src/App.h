#pragma once

#include "Renderer.h"
#include "Window.h"
#include <glad/glad.h>
#include <iostream>

class App {
    Window window;
    Renderer renderer;

  public:
    App();
    inline void onResize(int width, int height) {
        renderer.onResize(width, height);
    }
    void run();
    void processInput();
};
