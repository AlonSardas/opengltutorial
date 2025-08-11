#pragma once

#include "Mouse.h"
#include "PerspectiveProjection.h"
#include "QuatCamera.h"
#include "Window.h"
#include "renderers/IRenderer.h"
#include <memory>

class App {
  public:
    App();
    void onResize(int width, int height);
    void onMouseMove(double xposIn, double yposIn);
    void onMouseScroll(double xoffset, double yoffset);
    void run();

  private:
    void processInput(float deltaTime);

    Window window;
    QuatCamera camera;
    PerspectiveProjection projection;
    Mouse mouse;
    std::unique_ptr<IRenderer> renderer;

    // TODO: Inlcude mouse object
    bool firstMouse = true;
    float lastX = 0.0, lastY = 0.0;

    static constexpr int INITIAL_WIDTH = 1024;
    static constexpr int INITIAL_HEIGHT = 768;
};
