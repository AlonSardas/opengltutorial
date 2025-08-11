/*
https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/6.1.coordinate_systems/coordinate_systems.cpp

https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/6.3.coordinate_systems_multiple/coordinate_systems_multiple.cpp

https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.3.camera_mouse_zoom/camera_mouse_zoom.cpp
*/
#include "App.h"
#include "renderers/LightingRenderer.h"
#include "renderers/Renderer.h"

App::App()
    : window(INITIAL_WIDTH, INITIAL_HEIGHT, "My openGL app"), camera(),
      projection(45.0f, float(INITIAL_WIDTH) / INITIAL_HEIGHT) {
    // renderer = std::make_unique<Renderer>(camera, projection);
    renderer = std::make_unique<LightingRenderer>(camera, projection);
    renderer->init();
    renderer->onResize(INITIAL_WIDTH, INITIAL_HEIGHT);
    window.setAppForCallback(this);
}

void App::run() {
    float lastTime = glfwGetTime();

    while (!window.shouldClose()) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(deltaTime);

        renderer->render();

        window.swapBuffers();
        glfwPollEvents();
    }
}

void App::onResize(int width, int height) {
    renderer->onResize(width, height);
    projection.setAspectRatio(float(width) / height);
}

void App::processInput(float deltaTime) {
    const float moveSpeed = 3.0f * deltaTime;  // units per second
    const float turnSpeed = 45.0f * deltaTime; // degrees per second

    if (window.getKeyStatus(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        window.setShouldClose(true);
    }

    // --- Movement (WASD + RF) ---
    if (window.getKeyStatus(GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(moveSpeed);
    if (window.getKeyStatus(GLFW_KEY_S) == GLFW_PRESS)
        camera.moveForward(-moveSpeed);
    if (window.getKeyStatus(GLFW_KEY_A) == GLFW_PRESS)
        camera.moveRight(-moveSpeed);
    if (window.getKeyStatus(GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(moveSpeed);
    if (window.getKeyStatus(GLFW_KEY_F) == GLFW_PRESS)
        camera.moveUp(-moveSpeed);
    if (window.getKeyStatus(GLFW_KEY_R) == GLFW_PRESS)
        camera.moveUp(moveSpeed);

    // --- Rotation (Arrow keys) ---
    if (window.getKeyStatus(GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.rotateRight(turnSpeed);
    if (window.getKeyStatus(GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.rotateLeft(turnSpeed);
    if (window.getKeyStatus(GLFW_KEY_UP) == GLFW_PRESS)
        camera.rotateUp(turnSpeed);
    if (window.getKeyStatus(GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.rotateDown(turnSpeed);
    if (window.getKeyStatus(GLFW_KEY_Q) == GLFW_PRESS)
        camera.rollLeft(turnSpeed);
    if (window.getKeyStatus(GLFW_KEY_E) == GLFW_PRESS)
        camera.rollRight(turnSpeed);
}

void App::onMouseMove(double xposIn, double yposIn) {
    auto [dx, dy] = mouse.onMouseMove(xposIn, yposIn);
    // std::cout << dx << ", " << dy << std::endl;
    camera.rotate(dx, dy);
}

void App::onMouseScroll(double xoffset, double yoffset) {
    auto [dx, dy] = mouse.onMouseScroll(xoffset, yoffset);
    projection.adjustFov(-dy);
}
