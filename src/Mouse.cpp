#include "Mouse.h"

Mouse::Mouse()
    : firstMouse(true), lastX(0.0f), lastY(0.0f), sensitivity(0.2f),
      scrollSensitivity(3.0f) {}

std::pair<float, float> Mouse::onMouseMove(double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return {0.0f, 0.0f};
    }

    float dx = (xpos - lastX) * sensitivity;
    float dy = (lastY - ypos) * sensitivity;

    lastX = xpos;
    lastY = ypos;

    return {dx, dy};
}

std::pair<float, float> Mouse::onMouseScroll(double xoffset, double yoffset) {
    float dx = xoffset * scrollSensitivity;
    float dy = yoffset * scrollSensitivity;
    return {dx, dy};
}
