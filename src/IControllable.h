#pragma once

#include <glm/glm.hpp>

class IControllable {
  public:
    virtual ~IControllable() = default;

    virtual void moveForward(float deltaTime) = 0;
    virtual void moveBackward(float deltaTime) = 0;
    virtual void moveRight(float deltaTime) = 0;
    virtual void moveLeft(float deltaTime) = 0;
    virtual void moveUp(float deltaTime) = 0;
    virtual void moveDown(float deltaTime) = 0;
    virtual void move(const glm::vec3 &delta) = 0;
    virtual void setPosition(const glm::vec3 &pos) = 0;

    virtual void setRotation(float yawDegrees, float pitchDegrees, float rollDegrees) = 0;
    virtual void rotate(float dx, float dy, float dz) = 0;
    virtual void rotateRight(float deltaTime) = 0;
    virtual void rotateLeft(float deltaTime) = 0;
    virtual void rotateUp(float deltaTime) = 0;
    virtual void rotateDown(float deltaTime) = 0;
    virtual void rollLeft(float deltaTime) = 0;
    virtual void rollRight(float deltaTime) = 0;

    virtual const glm::mat4 &getViewMatrix() const = 0;
    // For players with body, this is different from getPosition()
    virtual const glm::vec3 &getCameraPosition() const = 0;
    virtual const glm::vec3 &getPosition() const = 0;
    virtual const glm::vec3 &getFront() const = 0;
};