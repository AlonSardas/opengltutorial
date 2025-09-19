#pragma once

#include "POVPlayer.h"

class Player : public POVPlayer {
  public:
    Player(const glm::vec3 &startPos = glm::vec3(0, 0, 0), float moveSpeed = 6.0f, float turnSpeed = 45.0f)
        : POVPlayer(startPos, moveSpeed, turnSpeed) {
        // Note: parent constructor only calls its own updateVectors and updateViewMatrix
        updateViewMatrix();
    }

  protected:
    void updateViewMatrix() override;

  private:
    const glm::vec3 cameraBase = glm::vec3(0.0f, 1.5f, 0.0f);
};