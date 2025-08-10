#pragma once

#include "EulerCamera.h"
#include "Mouse.h"
#include "QuatCamera.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include <glad/glad.h>
#include <iostream>
#include <stdexcept>

class App {
  public:
    App();
    void onResize(int width, int height) { renderer.onResize(width, height); }
    void onMouseMove(double xposIn, double yposIn);
    void onMouseScroll(double xoffset, double yoffset);
    void run();

  private:
    void processInput(float deltaTime);

    Window window;
    Renderer renderer;
    QuatCamera camera;
    Mouse mouse;

    // TODO: Inlcude mouse object
    bool firstMouse = true;
    float lastX = 0.0, lastY = 0.0;
};
