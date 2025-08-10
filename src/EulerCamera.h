#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EulerCamera {
  public:
    EulerCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
                float yaw = -90.0f, float pitch = 0.0f);

    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);

    void rotate(float yawOffset, float pitchOffset);
    void rotateRight(float amount) { rotate(amount, 0.0f); }
    void rotateLeft(float amount) { rotate(-amount, 0.0f); }
    void rotateUp(float amount) { rotate(0.0f, amount); }
    void rotateDown(float amount) { rotate(0.0f, -amount); }

    const glm::mat4 &getViewMatrix() const { return viewMatrix; }

  private:
    void updateCameraVectors();
    void updateViewMatrix();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    glm::mat4 viewMatrix;
};