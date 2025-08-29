#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PerspectiveProjection {
  public:
    PerspectiveProjection(float fovDegrees = 45.0f, float aspect = 4.0f / 3.0f, float nearPlane = 0.1f,
                          float farPlane = 100.0f);
    PerspectiveProjection(const PerspectiveProjection &) = delete;
    PerspectiveProjection &operator=(const PerspectiveProjection &) = delete;
    PerspectiveProjection(PerspectiveProjection &&) = delete;
    PerspectiveProjection &operator=(PerspectiveProjection &&) = delete;

    void setFov(float degrees);
    void adjustFov(float deltaDegrees);
    float getFov() const;

    void setAspectRatio(float ratio);
    float getAspectRatio() const;

    void setClippingPlanes(float nearPlane, float farPlane);

    glm::mat4 getMatrix() const;

  private:
    void updateProjectionMatrix();
    glm::mat4 projection;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    static constexpr float MIN_FOV = 1.0f;
    static constexpr float MAX_FOV = 70.0f;
};