#pragma once

#include "BaseControllable.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class QuatCamera : public BaseControllable {
  public:
    QuatCamera(glm::vec3 startPos = glm::vec3(0, 0, 3), float moveSpeed = 6.0f, float turnSpeed = 45.0f);

    void move(const glm::vec3 &delta) override;
    void rotate(float yawDegrees, float pitchDegrees, float rollDegrees) override;

    const glm::vec3 &getPosition() const { return position; }
    const glm::vec3 &getFront() const override;
    const glm::mat4 &getViewMatrix() const { return viewMatrix; }

    void setPosition(const glm::vec3 &pos) override { assert(false && "setPosition() not implemented"); };
    void setRotation(float yawDegrees, float pitchDegrees, float rollDegrees) override {
        assert(false && "setRotation() not implemented");
    };

  protected:
    void updateViewMatrix();

    glm::vec3 position;
    glm::vec3 front;
    glm::quat orientation;
    glm::mat4 viewMatrix;
};