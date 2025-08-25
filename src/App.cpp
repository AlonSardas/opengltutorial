/*
https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/6.1.coordinate_systems/coordinate_systems.cpp

https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/6.3.coordinate_systems_multiple/coordinate_systems_multiple.cpp

https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.3.camera_mouse_zoom/camera_mouse_zoom.cpp
*/

#include "App.h"
#include "renderers/LightingRenderer.h"
#include "renderers/MirrorsRenderer.h"
#include "renderers/ModelLoadingExample.h"
#include "renderers/Renderer.h"
#include "renderers/StencilGlowExample.h"
#include "stb/stb_image_write.h"

void saveScreenshot(const std::string &filename);

App::App()
    : window(INITIAL_WIDTH, INITIAL_HEIGHT, "My openGL app"), camera(),
      projection(45.0f, float(INITIAL_WIDTH) / INITIAL_HEIGHT) {
    // renderer = std::make_unique<Renderer>(camera, projection);
    // renderer = std::make_unique<LightingRenderer>(camera, projection);
    // renderer = std::make_unique<ModelLoadingExample>(camera, projection);
    // renderer = std::make_unique<StencilGlowExample>(camera, projection);
    renderer = std::make_unique<MirrorsRenderer>(camera, projection);
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
    const float moveSpeed = 4.5f * deltaTime;  // units per second
    const float turnSpeed = 45.0f * deltaTime; // degrees per second

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
    if (cursorEnabled) {
        return;
    }
    auto [dx, dy] = mouse.onMouseMove(xposIn, yposIn);
    // std::cout << "Mouse moved" << xposIn << ", " << yposIn << " dx=" << dx << " dy=" << dy << std::endl;
    camera.rotate(dx, dy);
}

void App::onMouseScroll(double xoffset, double yoffset) {
    if (cursorEnabled) {
        return;
    }
    auto [dx, dy] = mouse.onMouseScroll(xoffset, yoffset);
    projection.adjustFov(-dy);
}

void App::onMouseButton(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (cursorEnabled) {
            cursorEnabled = false;
            glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void App::onKeyChanged(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            if (!cursorEnabled) {
                cursorEnabled = true;
                glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                mouse.resetPosition();
            }
            break;
        case GLFW_KEY_F11:
            // mouse.resetPosition();
            window.toggleFullscreen();
            // mouse.resetPosition();
            break;
        case GLFW_KEY_P:
            saveScreenshot("../screenshot.png");
            break;
        }
    }
}

void saveScreenshot(const std::string &filename) {
    // https://vallentin.dev/blog/post/opengl-screenshot

    std::cout << "Capturing screenshot" << std::endl;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];
    std::vector<unsigned char> pixels(width * height * 3);

    // Read pixels from framebuffer (BGR format)
    glPixelStorei(GL_PACK_ALIGNMENT, 1); // Make sure rows are packed tightly
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Flip the image vertically
    for (int y = 0; y < height / 2; ++y) {
        int opposite = height - y - 1;
        for (int x = 0; x < width * 3; ++x) {
            std::swap(pixels[y * width * 3 + x], pixels[opposite * width * 3 + x]);
        }
    }

    // Write to PNG using stb_image_write
    if (stbi_write_png(filename.c_str(), width, height, 3, pixels.data(), width * 3)) {
        std::cout << "Screenshot saved to " << filename << "\n";
    } else {
        std::cerr << "Failed to save screenshot to " << filename << "\n";
    }
}