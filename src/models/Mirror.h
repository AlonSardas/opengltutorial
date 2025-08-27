#pragma once

#include "Quad.h"
#include "graphics/Framebuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mirror {
  public:
    Mirror(const glm::vec3 &pos, const glm::vec3 &normal, float width, float height, int textureWidth = 256,
           int textureHeight = 256);

    void update(const glm::vec3 &cameraPos);

    void beginMirrorRender(int screenWidth, int screenHeight);
    void endMirrorRender();

    bool shouldRender(const glm::vec3 &cameraPos) const;

    const glm::mat4 &getViewMatrix() const { return mirrorView; }
    const glm::mat4 &getProjectionMatrix() const { return mirrorProjection; }
    const glm::vec3 &getPosition() const { return position; }
    const glm::vec3 &getNormal() const { return normal; }
    const glm::vec3 &getReflectedPos() const { return reflectedCameraPos; }
    GLuint getTexture() const { return framebuffer.getTexture(); }

    void setPosition(const glm::vec3 &pos) { position = pos; }
    void setNormal(const glm::vec3 &norm) { normal = glm::normalize(norm); }
    void setRenderDistance(float distance) { renderDistance = distance; }
    void setActive(bool isActive) { active = isActive; }

    void draw(const glm::mat4 &viewMatrix, const glm::mat4 &mainProjection);

  private:
    Framebuffer framebuffer;

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 upVector;
    glm::mat3 mirrorRotation;

    float mirrorWidth;
    float mirrorHeight;

    float renderDistance;
    bool active;

    glm::vec3 reflectedCameraPos;
    glm::mat4 mirrorView;
    glm::mat4 mirrorProjection;

    Quad quad;
    Shader shader;

    void updateMatrices(const glm::vec3 &cameraPos);
    void init(int textureWidth = 256, int textureHeight = 256);
};