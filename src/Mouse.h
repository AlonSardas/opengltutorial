#pragma once

#include <utility>

class Mouse {
  public:
    Mouse();

    // Call from your event callback
    std::pair<float, float> onMouseMove(double xpos, double ypos);
    std::pair<float, float> onMouseScroll(double xoffset, double yoffset);

    void setSensitivity(float s) { sensitivity = s; }
    float getSensitivity() const { return sensitivity; }
    void setScrollSensitivity(float s) { scrollSensitivity = s; }
    float getScrollSensitivity() const { return scrollSensitivity; }
    void resetPosition() { firstMouse = true; }

  private:
    bool firstMouse;
    float lastX, lastY;
    float sensitivity;
    float scrollSensitivity;
};