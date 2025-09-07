#pragma once

#include "IControllable.h"

class BaseControllable : public IControllable {
  public:
    BaseControllable(float moveSpeed, float turnSpeed) : moveSpeed(moveSpeed), turnSpeed(turnSpeed) {}

    void moveForward(float deltaTime) override { move(glm::vec3(0, 0, deltaTime * moveSpeed)); }
    void moveBackward(float deltaTime) override { move(glm::vec3(0, 0, -deltaTime * moveSpeed)); }
    void moveRight(float deltaTime) override { move(glm::vec3(deltaTime * moveSpeed, 0, 0)); }
    void moveLeft(float deltaTime) override { move(glm::vec3(-deltaTime * moveSpeed, 0, 0)); }
    void moveUp(float deltaTime) override { move(glm::vec3(0, deltaTime * moveSpeed, 0)); }
    void moveDown(float deltaTime) override { move(glm::vec3(0, -deltaTime * moveSpeed, 0)); }
    void rotateRight(float deltaTime) override { rotate(deltaTime * turnSpeed, 0, 0); }
    void rotateLeft(float deltaTime) override { rotate(-deltaTime * turnSpeed, 0, 0); }
    void rotateUp(float deltaTime) override { rotate(0, deltaTime * turnSpeed, 0); }
    void rotateDown(float deltaTime) override { rotate(0, -deltaTime * turnSpeed, 0); }
    void rollLeft(float deltaTime) override { rotate(0, 0, -deltaTime * turnSpeed); }
    void rollRight(float deltaTime) override { rotate(0, 0, deltaTime * turnSpeed); }

  private:
    float moveSpeed;
    float turnSpeed;
};