#pragma once

#include "Mouse.h"
#include "cameras/QuatCamera.h"
#include "graphics/PerspectiveProjection.h"
#include "graphics/Window.h"
#include "renderers/IRenderer.h"
#include <memory>

class App {
  public:
    App();
    void onResize(int width, int height);
    void onMouseMove(double xposIn, double yposIn);
    void onMouseScroll(double xoffset, double yoffset);
    void onMouseButton(int button, int action, int mods);
    void onKeyChanged(int key, int scancode, int action, int mods);
    void run();

  private:
    void processInput(float deltaTime);
    void toggleFullscreen();

    Window window;
    QuatCamera camera;
    PerspectiveProjection projection;
    Mouse mouse;
    std::unique_ptr<IRenderer> renderer;
    bool cursorEnabled = false;
    bool isFullscreen = false;

    static constexpr int INITIAL_WIDTH = 1024;
    static constexpr int INITIAL_HEIGHT = 768;
};
