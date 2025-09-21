#pragma once

#include "BaseControllable.h"
#include "PortalTraversable.h"
#include "models/Portal.h"

class POVPlayer : public BaseControllable {
  public:
    POVPlayer(const glm::vec3 &startPos = glm::vec3(0, 0, 0), float moveSpeed = 6.0f, float turnSpeed = 45.0f);
    void rotate(float yawDegrees, float pitchDegrees, float rollDegrees) override;
    void move(const glm::vec3 &delta) override;

    void setPosition(const glm::vec3 &pos) override { position = pos; };
    void setRotation(float yawDegrees, float pitchDegrees, float rollDegrees) override {
        yaw = yawDegrees;
        pitch = pitchDegrees;
    };

    const glm::vec3 &getFront() const override { return front; }
    const glm::vec3 &getPosition() const override { return position; }
    const glm::vec3 &getCameraPosition() const override { return position; }
    const glm::mat4 &getViewMatrix() const override { return viewMatrix; }

    void addPortal(const Portal *portal) { portalComponent.addPortal(portal); }

  protected:
    void updateVectors();
    virtual void updateViewMatrix();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::mat4 viewMatrix;

    float yaw;
    float pitch;

    PortalTraversable portalComponent;
};