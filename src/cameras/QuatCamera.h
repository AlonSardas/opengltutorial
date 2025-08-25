#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class QuatCamera {
  public:
    QuatCamera(glm::vec3 startPos = glm::vec3(0, 0, 3));

    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);

    void rotate(float yawDegrees, float pitchDegrees, float rollDegrees = 0.0f);
    void rotateRight(float amount) { rotate(amount, 0.0f); }
    void rotateLeft(float amount) { rotate(-amount, 0.0f); }
    void rotateUp(float amount) { rotate(0.0f, amount); }
    void rotateDown(float amount) { rotate(0.0f, -amount); }
    void rollLeft(float amount) { rotate(0.0f, 0.0f, amount); }
    void rollRight(float amount) { rotate(0.0f, 0.0f, -amount); }

    const glm::vec3 &getPosition() const { return position; }
    glm::vec3 getFront() const;
    const glm::mat4 &getViewMatrix() const { return viewMatrix; }

  private:
    void updateViewMatrix();

    glm::vec3 position;
    glm::quat orientation;
    glm::mat4 viewMatrix;
};